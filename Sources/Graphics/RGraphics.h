#pragma once

#include "../RBase.h"

#include <map>


struct VkPipelineCache_T;
struct VkSemaphore_T;
struct VkFence_T;

namespace At0::Ray
{
	class Swapchain;
	class CommandPool;
	class CommandBuffer;
	class VulkanInstance;
	class PhysicalDevice;
	class Surface;
	class Renderer;
	class LogicalDevice;
	class Descriptor;

	class RAY_EXPORT Graphics
	{
	public:
		~Graphics();
		static Graphics& Get();

		const CommandPool& GetCommandPool(
			const std::thread::id& threadId = std::this_thread::get_id());
		const Swapchain& GetSwapchain() const { return *m_Swapchain.get(); }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice.get(); }
		const Surface& GetSurface() const { return *m_Surface.get(); }
		const LogicalDevice& GetLogicalDevice() const { return *m_LogicalDevice.get(); }

	private:
		Graphics();
		void CreatePipelineCache();

	private:
		// Scope<Renderer> m_Renderer;
		std::map<std::string, const Descriptor*> m_Attachments;
		Scope<Swapchain> m_Swapchain;

		std::map<std::thread::id, Scope<CommandPool>> m_CommandPools;

		VkPipelineCache_T* m_PipelineCache = 0;
		std::vector<VkSemaphore_T*> m_PresentCompletes;
		std::vector<VkSemaphore_T*> m_RenderCompletes;
		std::vector<VkFence_T*> m_FlightFences;
		size_t m_CurrentFrame = 0;
		bool m_FramebufferResized = false;

		std::vector<Scope<CommandBuffer>> m_CommandBuffers;

		Scope<VulkanInstance> m_Instance;
		Scope<PhysicalDevice> m_PhysicalDevice;
		Scope<Surface> m_Surface;
		Scope<LogicalDevice> m_LogicalDevice;
	};
}  // namespace At0::Ray
