#pragma once

#include "../../RBase.h"
#include "../../Utils/RThreadPool.h"
#include "../../Utils/RNonCopyable.h"

#include "RCommandPool.h"


namespace At0::Ray
{
	class Framebuffer;
	class RenderPass;
	class CommandBuffer;
	class SecondaryCommandBuffer;
	class RenderContext;

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
			uint32_t imageIndex, const VkViewport& viewport, const VkRect2D& scissor,
			UInt2 swapchainExtent);

		void WaitForTasks() { m_ThreadPool.WaitForTasks(); }
		void ResetCommandPools(uint32_t imageIndex) const;

		uint32_t GetThreadCount() const { return m_ThreadPool.GetThreadCount(); }
		const PrimaryResources& GetMainCommandResources(uint32_t imgIdx) const;
		auto& GetCommandResources(uint32_t imgIdx) const { return m_CommandResources[imgIdx]; }

	private:
		ThreadPool m_ThreadPool;
		std::vector<PrimaryResources> m_MainCommandResources;

		// [imageIndex][thread]
		std::vector<std::vector<SecondaryResources>> m_CommandResources;

		const RenderContext& m_Context;
	};
}  // namespace At0::Ray
