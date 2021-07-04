#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"
#include "../Core/RTime.h"
#include "Ray/Core/RMath.h"

#include "Ray/Events/REngineEvents.h"
#include "Ray/Events/REventListener.h"
#include "Ray/Events/REngineEvents.h"

#include <vulkan/vulkan_core.h>
#include <vector>
#include <array>

typedef struct RrFence_T* RrFence;
typedef struct RrSemaphore_T* RrSemaphore;
typedef struct RrPipelineCache_T* RrPipelineCache;

namespace At0::Ray
{
	class RendererInstance;
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


	class RAY_EXPORT Graphics : NonCopyable, EventListener<FramebufferResizedEvent>
	{
	public:
		~Graphics();
		static Graphics& Get();
		static void Destroy();

		const RendererInstance& GetInstance() const { return *m_RendererInstance; }
		const Surface& GetSurface() const { return *m_Surface; }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice; }
		const LogicalDevice& GetDevice() const { return *m_LogicalDevice; }
		const Swapchain& GetSwapchain() const { return *m_Swapchain; }
		const CommandPool& GetCommandPool() const { return *m_CommandPool; }
		const RenderPass& GetRenderPass() const { return *m_RenderPass; }
		RrPipelineCache GetPipelineCache() const { return m_PipelineCache; }
		static uint32_t GetImageCount() { return s_MaxFramesInFlight; }

		/**
		 * Acquires the next frame for rendering and presentation
		 * @param dt is the time since the last frame
		 */
		void Update(Delta dt);

	private:
		Graphics();

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
		void RecordCommandBuffer(
			const CommandBuffer& cmdBuff, const Framebuffer& framebuffer, uint32_t imageIndex);

		void OnEvent(FramebufferResizedEvent& e) override;
		void OnFramebufferResized();
		void WritePipelineCache();

	private:
		static Graphics* s_Instance;
		static constexpr uint8_t s_MaxFramesInFlight = 2;

		VkViewport m_Viewport{};
		VkRect2D m_Scissor{};

		Scope<RendererInstance> m_RendererInstance;
		Scope<PhysicalDevice> m_PhysicalDevice;
		Scope<Surface> m_Surface;
		Scope<LogicalDevice> m_LogicalDevice;
		Scope<Swapchain> m_Swapchain;
		Scope<CommandPool> m_CommandPool;
		Scope<RenderPass> m_RenderPass;
		RrPipelineCache m_PipelineCache = nullptr;
		Scope<DepthImage> m_DepthImage;

		std::vector<Scope<Framebuffer>> m_Framebuffers;
		std::vector<Scope<CommandBuffer>> m_CommandBuffers;
		Scope<CommandBufferRecorder> m_CommandBufferRecorder;

		std::array<RrFence, s_MaxFramesInFlight> m_InFlightFences;
		std::array<RrSemaphore, s_MaxFramesInFlight> m_ImageAvailableSemaphore;
		std::vector<RrFence> m_ImagesInFlight;
		std::array<RrSemaphore, s_MaxFramesInFlight> m_RenderFinishedSemaphore;

		uint32_t m_CurrentFrame = 0;
		bool m_FramebufferResized = false;
	};
}  // namespace At0::Ray
