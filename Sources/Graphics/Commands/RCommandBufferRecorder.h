#pragma once

#include "../../RBase.h"
#include "../../Utils/RThreadPool.h"
#include "../../Utils/RNonCopyable.h"


namespace At0::Ray
{
	class CommandPool;
	class CommandBuffer;
	class Framebuffer;
	class RenderPass;

	class RAY_EXPORT CommandBufferRecorder : NonCopyable
	{
	public:
		struct Resources
		{
			Scope<CommandPool> commandPool;
			Scope<CommandBuffer> commandBuffer;
		};

	public:
		CommandBufferRecorder(uint32_t numThreads, uint32_t numPools);
		~CommandBufferRecorder();

		void Record(const RenderPass& renderPass, const Framebuffer& framebuffer,
			uint32_t imageIndex, const VkViewport& viewport, const VkRect2D& scissor);

		void WaitForTasks() { m_ThreadPool.WaitForTasks(); }
		void FillSubmitInfo(uint32_t imageIndex, VkSemaphore imageAvailableSemaphore,
			VkSemaphore renderFinishedSemaphore, VkSubmitInfo& submitInfo);

		const std::vector<Resources>& GetCommandResources() const { return m_CommandResources; }

	private:
		ThreadPool m_ThreadPool;
		std::vector<Resources> m_CommandResources;

		std::vector<VkSemaphore> m_WaitSemaphores;
		std::vector<VkSemaphore> m_SignalSemaphores;
		std::vector<VkCommandBuffer> m_VkCommandBuffers;
	};
}  // namespace At0::Ray
