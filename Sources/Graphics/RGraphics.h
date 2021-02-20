#pragma once

#include "../RBase.h"
#include "../Utils/RNonCopyable.h"
#include "../Core/RTime.h"
#include "../Core/RMath.h"

#include <vulkan/vulkan_core.h>
#include <vector>


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
	class GraphicsPipeline;

	class RAY_EXPORT Graphics : NonCopyable
	{
	public:
		~Graphics();
		static Graphics& Get();

		const VulkanInstance& GetInstance() const { return *m_VulkanInstance; }
		const Surface& GetSurface() const { return *m_Surface; }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice; }
		const LogicalDevice& GetDevice() const { return *m_LogicalDevice; }
		const Swapchain& GetSwapchain() const { return *m_Swapchain; }
		const CommandPool& GetCommandPool() const { return *m_CommandPool; }

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
		void CreateCommandBuffers();
		void RecordCommandBuffer(const CommandBuffer& cmdBuff, const Framebuffer& framebuffer);
		void CreateSyncObjects();

	private:
		inline static Graphics* s_Instance = nullptr;

		VkViewport m_Viewport;
		VkRect2D m_Scissor;

		Scope<VulkanInstance> m_VulkanInstance;
		Scope<PhysicalDevice> m_PhysicalDevice;
		Scope<Surface> m_Surface;
		Scope<LogicalDevice> m_LogicalDevice;
		Scope<Swapchain> m_Swapchain;
		Scope<CommandPool> m_CommandPool;
		Scope<RenderPass> m_RenderPass;
		Scope<GraphicsPipeline> m_GraphicsPipeline;

		std::vector<Scope<Framebuffer>> m_Framebuffers;
		std::vector<Scope<CommandBuffer>> m_CommandBuffers;
	};
}  // namespace At0::Ray
