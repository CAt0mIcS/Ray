#pragma once

#include "../../Utils/RNonCopyable.h"
#include "../../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RenderContext;

	class RAY_EXPORT CommandPool : NonCopyable
	{
	public:
		CommandPool(RenderContext& context, VkCommandPoolCreateFlags flags = 0);
		~CommandPool();

		operator VkCommandPool() const { return m_CommandPool; }
		RenderContext& GetRenderContext() { return m_Context; }

	private:
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		RenderContext& m_Context;
	};
}  // namespace At0::Ray
