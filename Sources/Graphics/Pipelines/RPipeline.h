#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include "RShader.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Pipeline
	{
	public:
		virtual ~Pipeline();

		virtual VkPipelineBindPoint GetBindPoint() const = 0;

		const Shader& GetShader() const { return m_Shader; }
		const VkPipelineLayout& GetLayout() const { return m_Layout; }

		operator const VkPipeline&() const { return m_Pipeline; }

	protected:
		Pipeline() = default;

	protected:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_Layout;

		Shader m_Shader;
	};
}  // namespace At0::Ray
