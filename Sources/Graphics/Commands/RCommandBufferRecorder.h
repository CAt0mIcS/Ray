#pragma once

#include "../../RBase.h"
#include "../../Utils/RThreadPool.h"
#include "../../Utils/RNonCopyable.h"

#include "RCommandBuffer.h"
#include "RCommandPool.h"


namespace At0::Ray
{
	class Framebuffer;
	class RenderPass;

	class RAY_EXPORT CommandBufferRecorder : NonCopyable
	{
	public:
		struct SecondaryResources
		{
			Scope<CommandPool> commandPool;
			Scope<SecondaryCommandBuffer> commandBuffer;
		};

		struct PrimaryResources
		{
			Scope<CommandPool> commandPool;
			Scope<CommandBuffer> commandBuffer;
		};

	public:
		CommandBufferRecorder(uint32_t numThreads, uint32_t numPools, const RenderPass& renderPass,
			uint32_t subpassID, const std::vector<Scope<Framebuffer>>& framebuffers);
		~CommandBufferRecorder();

		void Record(const RenderPass& renderPass, const Framebuffer& framebuffer,
			uint32_t imageIndex, const VkViewport& viewport, const VkRect2D& scissor);

		void WaitForTasks() { m_ThreadPool.WaitForTasks(); }
		void ResetCommandPools(uint32_t imageIndex) const;

		uint32_t GetThreadCount() const { return m_ThreadPool.GetThreadCount(); }
		const CommandBuffer& GetMainCommandBuffer(uint32_t imgIdx) const;
		auto& GetVkCommandBuffers(uint32_t imgIdx) const { return m_VkCommandBuffers[imgIdx]; }
		auto& GetCommandResources(uint32_t imgIdx) const { return m_CommandResources[imgIdx]; }

	private:
		ThreadPool m_ThreadPool;
		std::vector<PrimaryResources> m_MainCommandResources;

		// [imageIndex][thread]
		std::vector<std::vector<SecondaryResources>> m_CommandResources;
		std::vector<std::vector<VkCommandBuffer>> m_VkCommandBuffers;
	};
}  // namespace At0::Ray
