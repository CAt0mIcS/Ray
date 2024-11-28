#include "RCommandPool.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RRenderContext.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	CommandPool::CommandPool(VkCommandPoolCreateFlags flags)
	{
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = flags;
		createInfo.queueFamilyIndex = Graphics::Get().GetRenderContext().device.GetGraphicsFamily();

		ThrowVulkanError(vkCreateCommandPool(Graphics::Get().GetRenderContext().device, &createInfo,
							 nullptr, &m_CommandPool),
			"[CommandPool] Failed to create");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(Graphics::Get().GetRenderContext().device, m_CommandPool, nullptr);
	}
}  // namespace At0::Ray
