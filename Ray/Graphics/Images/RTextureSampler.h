#pragma once

#include "Ray/RBase.h"
#include <RayRenderer/Core/RSampler.h>


namespace At0::Ray
{
	class RAY_EXPORT TextureSampler
	{
	public:
		TextureSampler(RrSamplerAddressMode addressModeU = RrSamplerAddressModeRepeat,
			RrSamplerAddressMode addressModeV = RrSamplerAddressModeRepeat,
			RrSamplerAddressMode addressModeW = RrSamplerAddressModeRepeat, float maxLod = 0.0f);
		~TextureSampler();

		operator RrSampler() const { return m_Sampler; }

	private:
		RrSampler m_Sampler;
	};
}  // namespace At0::Ray
