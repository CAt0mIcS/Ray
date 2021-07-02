#pragma once

#include "Ray/RBase.h"
#include "RImage.h"


namespace At0::Ray
{
	class Image2D : public Image
	{
	public:
		static Ref<Image2D> Acquire(UInt2 extent, RrFormat format, RrImageTiling tiling,
			RrImageUsageFlags usage, RrMemoryPropertyFlags memProps, uint32_t mipLevels = 1,
			RrImageAspectFlags imageAspect = RrImageAspectColor, uint32_t arrayLayers = 1,
			RrImageCreateFlags createFlags = 0);

		Image2D(UInt2 extent, RrFormat format, RrImageTiling tiling, RrImageUsageFlags usage,
			RrMemoryPropertyFlags memProps, uint32_t mipLevels = 1,
			RrImageAspectFlags imageAspect = RrImageAspectColor, uint32_t arrayLayers = 1,
			RrImageCreateFlags createFlags = 0);

	protected:
		Image2D() = default;

		void Setup(UInt2 extent, RrFormat format, RrImageTiling tiling, RrImageUsageFlags usage,
			RrMemoryPropertyFlags memProps, uint32_t mipLevels = 1,
			RrImageAspectFlags imageAspect = RrImageAspectColor, uint32_t arrayLayers = 1,
			RrImageCreateFlags createFlags = 0);
	};
}  // namespace At0::Ray
