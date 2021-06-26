#pragma once

#include "Ray/RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT TextureSampler
	{
	public:
		TextureSampler(VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			float maxLod = 0.0f);
		~TextureSampler();

		operator const VkSampler&() const { return m_Sampler; }

	private:
		VkSampler m_Sampler;
	};
}  // namespace At0::Ray
