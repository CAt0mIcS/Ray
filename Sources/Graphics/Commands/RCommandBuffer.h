#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class CommandPool;

	class RAY_EXPORT CommandBuffer
	{
	public:
		CommandBuffer(const CommandPool& commandPool,
			VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~CommandBuffer();

		void Begin(VkCommandBufferUsageFlags usageFlags);
		void End();

	private:
		VkCommandBuffer m_CommandBuffer;
		const CommandPool& m_CommandPool;
	};
}  // namespace At0::Ray
