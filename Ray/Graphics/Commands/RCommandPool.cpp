#include "RCommandPool.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RRenderContext.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	CommandPool::CommandPool(RenderContext& context, VkCommandPoolCreateFlags flags)
		: m_Context(context)
	{
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = flags;
		createInfo.queueFamilyIndex = m_Context.device.GetGraphicsFamily();

		ThrowVulkanError(
			vkCreateCommandPool(m_Context.device, &createInfo, nullptr, &m_CommandPool),
			"[CommandPool] Failed to create");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(m_Context.device, m_CommandPool, nullptr);
	}
}  // namespace At0::Ray
