#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class CommandPool;

	class RAY_EXPORT CommandBuffer : NonCopyable
	{
	public:
		CommandBuffer(const CommandPool& commandPool,
			VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~CommandBuffer();

		void Begin(VkCommandBufferUsageFlags usageFlags = 0) const;
		void End() const;

		operator const VkCommandBuffer&() const { return m_CommandBuffer; }

	private:
		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
		const CommandPool& m_CommandPool;
	};
}  // namespace At0::Ray
