#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class CommandPool;

	class RAY_EXPORT CommandBuffer
	{
	public:
		/**
		 * Creates a new command buffer
		 * @param begin If true {@link CommandBuffer#Begin} is called.
		 * @param queueType The queue to run this command bufferr on.
		 * @param bufferLevel The buffer level.
		 */
		CommandBuffer(bool begin = true, VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT,
			VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~CommandBuffer();

		/**
		 * Beginns the recording state for this command buffer
		 * @param usage How this command buffer wwill be used.
		 */
		void Begin(VkCommandBufferUsageFlags usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		/**
		 * Ends the recording state for this command buffer.
		 */
		void End();

		/**
		 * Submits the command buffer to the queue and will hold the current thread idle until it
		 * has finished.
		 */
		void SubmitIdle();

		/**
		 * Submits the command buffer.
		 * @param waitSemaphore A optional semaphore that will waited upon before the command buffer
		 * is executed.
		 * @param signalSemaphore A optional that is signaled once the command buffer has been
		 * executed.
		 * @param fence A optional fence that is signaled once the command buffer has completed.
		 */
		void Submit(const VkSemaphore& waitSemaphore = VK_NULL_HANDLE,
			const VkSemaphore& signalSemaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE);

		operator const VkCommandBuffer&() const { return m_CommandBuffer; }

		const VkCommandBuffer& GetCommandBufferr() const { return m_CommandBuffer; }
		bool IsRunning() const { return m_Running; }

	private:
		VkQueue GetQueue() const;

	private:
		const CommandPool& m_CommandPool;

		VkQueueFlagBits m_QueueType;
		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
		bool m_Running = false;
	};
}  // namespace At0::Ray
