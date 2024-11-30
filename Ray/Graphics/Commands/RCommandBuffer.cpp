#include "RCommandBuffer.h"

#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RRenderContext.h"
#include "RCommandPool.h"


namespace At0::Ray
{
	CommandBuffer::CommandBuffer(CommandPool& commandPool, VkCommandBufferLevel bufferLevel)
		: m_CommandPool(&commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = *m_CommandPool;
		allocInfo.level = bufferLevel;
		allocInfo.commandBufferCount = 1;

		ThrowVulkanError(vkAllocateCommandBuffers(m_CommandPool->GetRenderContext().device,
							 &allocInfo, &m_CommandBuffer),
			"[CommandBuffer] Failed to allocate command buffer");
	}

	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(
			m_CommandPool->GetRenderContext().device, *m_CommandPool, 1, &m_CommandBuffer);
	}

	void CommandBuffer::Begin(VkCommandBufferUsageFlags usageFlags) const
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usageFlags;
		beginInfo.pInheritanceInfo = GetInheritanceInfo();

		ThrowVulkanError(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo),
			"[CommandBuffer] Failed to begin recording");
	}

	void CommandBuffer::End() const
	{
		ThrowVulkanError(
			vkEndCommandBuffer(m_CommandBuffer), "[CommandBuffer] Failed to end recording");
	}

	VkResult CommandBuffer::Submit(VkQueue queue, VkFence fence) const
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

		return vkQueueSubmit(queue, 1, &submitInfo, fence);
	}

	void CommandBuffer::Execute(const SecondaryCommandBuffer& secCmdBuff) const
	{
		VkCommandBuffer buff = secCmdBuff;
		vkCmdExecuteCommands(m_CommandBuffer, 1, &buff);
	}

	SecondaryCommandBuffer& CommandBuffer::AddSecondary(
		VkCommandBufferInheritanceInfo inheritanceInfo)
	{
		return m_SecondaryCommandBuffers.emplace_back(*m_CommandPool, std::move(inheritanceInfo));
	}

	CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
	{
		m_CommandBuffer = other.m_CommandBuffer;
		m_CommandPool = other.m_CommandPool;
		m_SecondaryCommandBuffers = std::move(other.m_SecondaryCommandBuffers);
		return *this;
	}

	CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept
	{
		*this = std::move(other);
	}

	// -----------------------------------------------------------------------------------------------
	// Secondary Command Buffer
	SecondaryCommandBuffer::SecondaryCommandBuffer(
		CommandPool& commandPool, VkCommandBufferInheritanceInfo inheritanceInfo)
		: CommandBuffer(commandPool, VK_COMMAND_BUFFER_LEVEL_SECONDARY),
		  m_InheritanceInfo(std::move(inheritanceInfo))
	{
	}

	const VkCommandBufferInheritanceInfo* SecondaryCommandBuffer::GetInheritanceInfo() const
	{
		return &m_InheritanceInfo;
	}
}  // namespace At0::Ray
