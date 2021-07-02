#pragma once

#include "RTextureSampler.h"
#include "RImage2D.h"

#include <string_view>


namespace At0::Ray
{
	class RAY_EXPORT Texture2D : public Image2D
	{
	public:
		static Ref<Texture2D> Acquire(std::string_view filepath);
		static Ref<Texture2D> Acquire(UInt2 extent, RrFormat format, RrImageTiling tiling,
			RrImageUsageFlags usage, RrMemoryPropertyFlags memProps,
			RrImageAspectFlags imageAspect = RrImageAspectColor);

		Texture2D(std::string_view filepath);
		Texture2D(UInt2 extent, RrFormat format, RrImageTiling tiling, RrImageUsageFlags usage,
			RrMemoryPropertyFlags memProps, RrImageAspectFlags imageAspect = RrImageAspectColor);

		const TextureSampler& GetSampler() const { return m_Sampler; }

	private:
		TextureSampler m_Sampler;
	};

}  // namespace At0::Ray
