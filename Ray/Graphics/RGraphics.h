#pragma once

#include "../RBase.h"
#include "../Utils/RNonCopyable.h"
#include "../Core/RTime.h"

#include "../Events/REventListener.h"
#include "../Events/REngineEvents.h"

#include <vulkan/vulkan_core.h>

#include <vector>
#include <array>


namespace At0::Ray
{
	class VulkanInstance;
	class PhysicalDevice;
	class Surface;
	class LogicalDevice;
	class Swapchain;
	class CommandPool;
	class CommandBuffer;
	class RenderPass;
	class Framebuffer;
	class DepthImage;
	class CommandBufferRecorder;
	class ShadowMappingObjects;
	class Window;
	class RenderContext;
	class Scene;


	class RAY_EXPORT Graphics : NonCopyable, EventListener<FramebufferResizedEvent>
	{
	public:
		Graphics(Window& window, RenderContext& context);
		~Graphics();
		static Graphics& Get();

		const RenderContext& GetRenderContext() const { return m_Context; }
		const Swapchain& GetSwapchain() const { return *m_Swapchain; }
		Ref<CommandPool> GetTransientCommandPool() { return m_TransientCommandPool; }
		const RenderPass& GetRenderPass() const { return *m_RenderPass; }
		VkPipelineCache GetPipelineCache() const { return m_PipelineCache; }
		static uint32_t GetImageCount() { return s_MaxFramesInFlight; }

		const ShadowMappingObjects& GetShadowMapping() { return *m_ShadowMapping; }

		/**
		 * Acquires the next frame for rendering and presentation
		 * @param dt is the time since the last frame
		 */
		void Update(Delta dt);

	private:
		void UpdateViewport();
		void UpdateScissor();

		// -------------------------------------------------------------
		// Vulkan object creation functions
		void CreateVulkanObjects();
		void CreateRenderPass();
		void CreateFramebuffers();
		void CreatePipelineCache();
		void CreateSyncObjects();
		void CreateCommandBuffers();
		void RecordCommandBuffer(Scene& lockedScene, const CommandBuffer& cmdBuff,
			const Framebuffer& framebuffer, uint32_t imageIndex);

		void OnEvent(FramebufferResizedEvent& e) override;
		void OnFramebufferResized(Scene& lockedScene);
		void WritePipelineCache();

	private:
		static Graphics* s_Instance;
		static constexpr uint8_t s_MaxFramesInFlight = 2;

		VkViewport m_Viewport{};
		VkRect2D m_Scissor{};

		Window& m_Window;

		RenderContext& m_Context;

		Scope<Swapchain> m_Swapchain;
		Scope<CommandPool> m_CommandPool;
		Ref<CommandPool> m_TransientCommandPool;
		Scope<RenderPass> m_RenderPass;
		VkPipelineCache m_PipelineCache = nullptr;
		Scope<DepthImage> m_DepthImage;

		std::vector<Scope<Framebuffer>> m_Framebuffers;
		std::vector<Scope<CommandBuffer>> m_CommandBuffers;
		Scope<CommandBufferRecorder> m_CommandBufferRecorder;

		Scope<ShadowMappingObjects> m_ShadowMapping;

		std::array<VkFence, s_MaxFramesInFlight> m_InFlightFences;
		std::array<VkSemaphore, s_MaxFramesInFlight> m_ImageAvailableSemaphore;
		std::vector<VkFence> m_ImagesInFlight;
		std::array<VkSemaphore, s_MaxFramesInFlight> m_RenderFinishedSemaphore;

		uint32_t m_CurrentFrame = 0;
		bool m_FramebufferResized = false;
	};
}  // namespace At0::Ray
