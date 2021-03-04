#pragma once

#include "../../RBase.h"
#include "RImage.h"


namespace At0::Ray
{
	class Image2D : public Image
	{
	public:
		Image2D(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT);
	};
}  // namespace At0::Ray
