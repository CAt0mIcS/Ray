#pragma once

#include "../../RBase.h"
#include "RImage.h"


namespace At0::Ray
{
	class Image2D : public Image
	{
	public:
		Image2D(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps, uint32_t mipLevels = 1,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT, uint32_t arrayLayers = 1,
			VkImageCreateFlags createFlags = 0);

	protected:
		Image2D() = default;

		void Setup(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps, uint32_t mipLevels = 1,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT, uint32_t arrayLayers = 1,
			VkImageCreateFlags createFlags = 0);
	};
}  // namespace At0::Ray
