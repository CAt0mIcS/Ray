#include "Rpch.h"
#include "RCommandPool.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	CommandPool::CommandPool(VkCommandPoolCreateFlags flags)
	{
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = flags;
		createInfo.queueFamilyIndex = Graphics::Get().GetDevice().GetGraphicsFamily();

		ThrowVulkanError(
			vkCreateCommandPool(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_CommandPool),
			"[CommandPool] Failed to create");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(Graphics::Get().GetDevice(), m_CommandPool, nullptr);
	}
}  // namespace At0::Ray
