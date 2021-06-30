#pragma once

#include "Ray/RBase.h"
#include "RayBase/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Core/RCommandBuffer.h>


namespace At0::Ray
{
	class CommandPool;
	class SecondaryCommandBuffer;

	class RAY_EXPORT CommandBuffer : NonCopyable
	{
	public:
		CommandBuffer(const CommandPool& commandPool,
			RrCommandBufferLevel bufferLevel = RrCommandBufferLevelPrimary);
		virtual ~CommandBuffer();

		void Begin(RrCommandBufferUsageFlags usageFlags = 0) const;
		void End() const;

		RrError Submit(RrQueue queue, RrFence fence = nullptr) const;
		void Execute(const SecondaryCommandBuffer& secCmdBuff) const;

		SecondaryCommandBuffer& AddSecondary(RrCommandBufferInheritanceInfo inheritanceInfo);
		const auto& GetSecondaryCommandBuffers() const { return m_SecondaryCommandBuffers; }
		virtual const RrCommandBufferInheritanceInfo* GetInheritanceInfo() const { return nullptr; }

		operator VkCommandBuffer() const { return (VkCommandBuffer)m_CommandBuffer; }
		operator RrCommandBuffer() const { return m_CommandBuffer; }

		CommandBuffer& operator=(CommandBuffer&& other) noexcept;
		CommandBuffer(CommandBuffer&& other) noexcept;

	private:
		RrCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
		const CommandPool* m_CommandPool;

		std::vector<SecondaryCommandBuffer> m_SecondaryCommandBuffers;
	};


	class RAY_EXPORT SecondaryCommandBuffer : public CommandBuffer
	{
	public:
		SecondaryCommandBuffer(
			const CommandPool& commandPool, RrCommandBufferInheritanceInfo inheritanceInfo);

		const RrCommandBufferInheritanceInfo* GetInheritanceInfo() const override;

	private:
		RrCommandBufferInheritanceInfo m_InheritanceInfo{};
	};
}  // namespace At0::Ray
