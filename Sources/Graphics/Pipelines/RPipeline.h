#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Pipeline
	{
	public:
		virtual ~Pipeline();

		virtual VkPipelineBindPoint GetBindPoint() const = 0;

		const VkPipelineLayout& GetLayout() const { return m_Layout; }
		operator const VkPipeline&() const { return m_Pipeline; }

	protected:
		Pipeline() = default;

	protected:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;
	};
}  // namespace At0::Ray
