#pragma once

#include "../RBase.h"
#include "../Utils/RNonCopyable.h"

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
	class RenderPass;
	class Framebuffer;

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

	private:
		Graphics();

		// -------------------------------------------------------------
		// Vulkan object creation functions
		void CreateVulkanObjects();
		void CreateRenderPass();
		void CreateFramebuffers();

	private:
		inline static Graphics* s_Instance = nullptr;

		Scope<VulkanInstance> m_VulkanInstance;
		Scope<PhysicalDevice> m_PhysicalDevice;
		Scope<Surface> m_Surface;
		Scope<LogicalDevice> m_LogicalDevice;
		Scope<Swapchain> m_Swapchain;
		Scope<CommandPool> m_CommandPool;
		Scope<RenderPass> m_RenderPass;

		std::vector<Scope<Framebuffer>> m_Framebuffers;
	};
}  // namespace At0::Ray
