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

		void Begin();
		void End();

	private:
		VkCommandBuffer m_Buffer;
	};
}  // namespace At0::Ray
