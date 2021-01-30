#include "Rpch.h"
#include "RCommandPool.h"

#include "Devices/Vulkan/RLogicalDevice.h"
#include "Debug/RException.h"
#include "Graphics/RGraphics.h"


namespace At0::Ray
{
	CommandPool::CommandPool(std::thread::id threadID) : m_ThreadID(threadID)
	{
		const LogicalDevice& logicalDevice = Graphics::Get().GetLogicalDevice();
		uint32_t graphicsFamily = logicalDevice.GetGraphicsFamily();

		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags =
			VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = graphicsFamily;
		RAY_VK_THROW_FAILED(
			vkCreateCommandPool(logicalDevice, &createInfo, nullptr, &m_CommandPool),
			"[CommandPool] Failed to create on thread {0}.", m_ThreadID);
	}

	CommandPool::~CommandPool()
	{
		const LogicalDevice& logicalDevice = Graphics::Get().GetLogicalDevice();
		vkDestroyCommandPool(logicalDevice, m_CommandPool, nullptr);
	}
}  // namespace At0::Ray
