#include "Rpch.h"
#include "RCommandBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "RCommandPool.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	CommandBuffer::CommandBuffer(const CommandPool& commandPool, VkCommandBufferLevel bufferLevel)
		: m_CommandPool(commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = bufferLevel;
		allocInfo.commandBufferCount = 1;

		RAY_VK_THROW_FAILED(
			vkAllocateCommandBuffers(Graphics::Get().GetDevice(), &allocInfo, &m_CommandBuffer),
			"[CommandBuffer] Failed to allocate command buffer");
	}

	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(Graphics::Get().GetDevice(), m_CommandPool, 1, &m_CommandBuffer);
	}

	void CommandBuffer::Begin(VkCommandBufferUsageFlags usageFlags) const
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usageFlags;
		beginInfo.pInheritanceInfo = nullptr;

		RAY_VK_THROW_FAILED(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo),
			"[CommandBuffer] Failed to begin recording");
	}

	void CommandBuffer::End() const
	{
		RAY_VK_THROW_FAILED(
			vkEndCommandBuffer(m_CommandBuffer), "[CommandBuffer] Failed to end recording");
	}
}  // namespace At0::Ray
