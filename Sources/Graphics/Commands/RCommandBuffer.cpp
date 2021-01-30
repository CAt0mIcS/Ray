#include "Rpch.h"
#include "RCommandBuffer.h"

#include "RCommandPool.h"
#include "Devices/Vulkan/RLogicalDevice.h"
#include "Debug/RException.h"
#include "Graphics/RGraphics.h"

#include <vulkan/vulkan.h>


namespace At0::Ray
{
	CommandBuffer::CommandBuffer(
		bool begin, VkQueueFlagBits queueType, VkCommandBufferLevel bufferLevel)
		: m_CommandPool(Graphics::Get().GetCommandPool()), m_QueueType(queueType)
	{
		const LogicalDevice& device = Graphics::Get().GetLogicalDevice();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = bufferLevel;
		allocInfo.commandBufferCount = 1;
		RAY_VK_THROW_FAILED(vkAllocateCommandBuffers(device, &allocInfo, &m_CommandBuffer),
			"[CommandBuffer] Failed to allocate.");

		if (begin)
			Begin();
	}

	CommandBuffer::~CommandBuffer()
	{
		const LogicalDevice& device = Graphics::Get().GetLogicalDevice();
		vkFreeCommandBuffers(device, m_CommandPool, 1, &m_CommandBuffer);
	}

	void CommandBuffer::Begin(VkCommandBufferUsageFlags usage)
	{
		if (m_Running)
			return;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;
		RAY_VK_THROW_FAILED(
			vkBeginCommandBuffer(m_CommandBuffer, &beginInfo), "[CommandBuffer] Failed to begin.");
		m_Running = true;
	}

	void CommandBuffer::End()
	{
		if (!m_Running)
			return;
		RAY_VK_THROW_FAILED(vkEndCommandBuffer(m_CommandBuffer), "[CommandBuffer] Failed to end.");
		m_Running = false;
	}

	void CommandBuffer::SubmitIdle()
	{
		const LogicalDevice& device = Graphics::Get().GetLogicalDevice();
		VkQueue queue = GetQueue();

		if (m_Running)
			End();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		VkFence fence;
		RAY_VK_THROW_FAILED(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence),
			"[CommandBuffer] Failed to create fence.");

		RAY_VK_THROW_FAILED(
			vkResetFences(device, 1, &fence), "[CommandBuffer] Failed to reset fence.");

		RAY_VK_THROW_FAILED(
			vkQueueSubmit(queue, 1, &submitInfo, fence), "[CommandBuffer] Failed to submit queue.");

		RAY_VK_THROW_FAILED(
			vkWaitForFences(device, 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max()),
			"[CommandBuffer] Failed to wait for fence.");

		vkDestroyFence(device, fence, nullptr);
	}

	void CommandBuffer::Submit(
		const VkSemaphore& waitSemaphore, const VkSemaphore& signalSemaphore, VkFence fence)
	{
		const LogicalDevice& device = Graphics::Get().GetLogicalDevice();
		VkQueue queue = GetQueue();

		if (m_Running)
			End();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

		if (waitSemaphore != VK_NULL_HANDLE)
		{
			static constexpr VkPipelineStageFlags submitPipelineStages =
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

			submitInfo.pWaitDstStageMask = &submitPipelineStages;
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &waitSemaphore;
		}

		if (signalSemaphore != VK_NULL_HANDLE)
		{
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &signalSemaphore;
		}

		if (fence != VK_NULL_HANDLE)
		{
			RAY_VK_THROW_FAILED(
				vkResetFences(device, 1, &fence), "[CommandBuffer] Failed to reset fence.");
		}

		RAY_VK_THROW_FAILED(
			vkQueueSubmit(queue, 1, &submitInfo, fence), "[CommandBuffer] Failed to submit queue.");
	}

	VkQueue CommandBuffer::GetQueue() const
	{
		const LogicalDevice& device = Graphics::Get().GetLogicalDevice();

		switch (m_QueueType)
		{
		case VK_QUEUE_GRAPHICS_BIT: return device.GetGraphicsQueue();
		case VK_QUEUE_COMPUTE_BIT: return device.GetComputeQueue();
		default: return nullptr;
		}
	}
}  // namespace At0::Ray
