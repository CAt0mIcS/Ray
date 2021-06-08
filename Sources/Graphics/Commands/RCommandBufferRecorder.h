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

		uint32_t GetThreadCount() const { return m_ThreadPool.GetThreadCount(); }
		auto& GetVkCommandBuffers(uint32_t imgIdx) const { return m_VkCommandBuffers[imgIdx]; }
		auto& GetCommandResources(uint32_t imgIdx) const { return m_CommandResources[imgIdx]; }

	private:
		ThreadPool m_ThreadPool;

		// [imageIndex][thread]
		std::vector<std::vector<Resources>> m_CommandResources;
		std::vector<std::vector<VkCommandBuffer>> m_VkCommandBuffers;
	};
}  // namespace At0::Ray
