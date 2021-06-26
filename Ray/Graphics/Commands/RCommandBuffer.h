#pragma once

#include "Ray/RBase.h"
#include "RayBase/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class CommandPool;
	class SecondaryCommandBuffer;

	class RAY_EXPORT CommandBuffer : NonCopyable
	{
	public:
		CommandBuffer(const CommandPool& commandPool,
			VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		virtual ~CommandBuffer();

		void Begin(VkCommandBufferUsageFlags usageFlags = 0) const;
		void End() const;

		VkResult Submit(VkQueue queue, VkFence fence = VK_NULL_HANDLE) const;
		void Execute(const SecondaryCommandBuffer& secCmdBuff) const;

		SecondaryCommandBuffer& AddSecondary(VkCommandBufferInheritanceInfo inheritanceInfo);
		const auto& GetSecondaryCommandBuffers() const { return m_SecondaryCommandBuffers; }
		virtual const VkCommandBufferInheritanceInfo* GetInheritanceInfo() const { return nullptr; }

		operator const VkCommandBuffer&() const { return m_CommandBuffer; }

		CommandBuffer& operator=(CommandBuffer&& other) noexcept;
		CommandBuffer(CommandBuffer&& other) noexcept;

	private:
		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
		const CommandPool* m_CommandPool;

		std::vector<SecondaryCommandBuffer> m_SecondaryCommandBuffers;
	};


	class RAY_EXPORT SecondaryCommandBuffer : public CommandBuffer
	{
	public:
		SecondaryCommandBuffer(
			const CommandPool& commandPool, VkCommandBufferInheritanceInfo inheritanceInfo);

		const VkCommandBufferInheritanceInfo* GetInheritanceInfo() const override;

	private:
		VkCommandBufferInheritanceInfo m_InheritanceInfo{};
	};
}  // namespace At0::Ray
