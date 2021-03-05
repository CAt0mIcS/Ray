#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT TextureSampler
	{
	public:
		TextureSampler();
		~TextureSampler();

		operator const VkSampler&() const { return m_Sampler; }

	private:
		VkSampler m_Sampler;
	};
}  // namespace At0::Ray
