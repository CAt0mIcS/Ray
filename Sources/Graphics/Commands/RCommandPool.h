#pragma once

#include "../../Utils/RNonCopyable.h"
#include "../../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT CommandPool : NonCopyable
	{
	public:
		CommandPool(VkCommandPoolCreateFlags flags = 0);
		~CommandPool();

		operator const VkCommandPool&() const { return m_CommandPool; }

	private:
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
	};
}  // namespace At0::Ray
