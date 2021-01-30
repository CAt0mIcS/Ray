#include "Rpch.h"
#include "RGraphics.h"

#include "Devices/Vulkan/RInstance.h"
#include "Devices/Vulkan/RLogicalDevice.h"
#include "Devices/Vulkan/RPhysicalDevice.h"
#include "Devices/Vulkan/RSurface.h"
#include "Commands/RCommandBuffer.h"
#include "Commands/RCommandPool.h"
#include "Renderpass/RSwapchain.h"
#include "RRenderer.h"

#include "Debug/RLogger.h"
#include "Debug/RException.h"


namespace At0::Ray
{
	Graphics::~Graphics()
	{
		VkQueue graphicsQueue = m_LogicalDevice->GetGraphicsQueue();
		if (vkQueueWaitIdle(graphicsQueue) != VK_SUCCESS)
			Log::Error("[Graphics] Failed to wait for graphics queue.");

		// RAY_TODO: Exception here
		vkDestroyPipelineCache(*m_LogicalDevice, m_PipelineCache, nullptr);

		for (size_t i = 0; i < m_FlightFences.size(); ++i)
		{
			vkDestroyFence(*m_LogicalDevice, m_FlightFences[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_RenderCompletes[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_PresentCompletes[i], nullptr);
		}

		m_CommandPools.clear();
		m_CommandBuffers.clear();

		m_Swapchain = nullptr;
		// m_Renderer = nullptr;
	}

	Graphics& Graphics::Get()
	{
		static Graphics graphics;
		return graphics;
	}

	const CommandPool& Graphics::GetCommandPool(const std::thread::id& threadId)
	{
		if (auto it = m_CommandPools.find(threadId); it != m_CommandPools.end())
			return *it->second;

		// RAY_TODO: Crash here
		return *m_CommandPools.emplace(threadId, MakeScope<CommandPool>(threadId)).first->second;
	}

	Graphics::Graphics()
		: m_Instance(MakeScope<VulkanInstance>()),
		  m_PhysicalDevice(MakeScope<PhysicalDevice>(m_Instance.get())),
		  m_Surface(MakeScope<Surface>(m_Instance.get(), m_PhysicalDevice.get())),
		  m_LogicalDevice(
			  MakeScope<LogicalDevice>(m_Instance.get(), m_PhysicalDevice.get(), m_Surface.get()))
	{
		CreatePipelineCache();
	}

	void Graphics::CreatePipelineCache()
	{
		VkPipelineCacheCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		RAY_VK_THROW_FAILED(
			vkCreatePipelineCache(*m_LogicalDevice, &createInfo, nullptr, &m_PipelineCache),
			"[Graphics] Failed to create pipeline cache.");
	}
}  // namespace At0::Ray
