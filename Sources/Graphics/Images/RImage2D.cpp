#include "Rpch.h"
#include "RImage2D.h"


namespace At0::Ray
{
	Image2D::Image2D(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProps, VkImageAspectFlags imageAspect)
		: Image(extent, VK_IMAGE_TYPE_2D, format, tiling, usage, memProps, imageAspect)
	{
	}
}  // namespace At0::Ray
