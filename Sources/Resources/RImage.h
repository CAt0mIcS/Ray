#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class RAY_EXPORT Image
	{
	public:
		void CreateImageView(const VkImage& image, VkImageView& imageVieww, VkImageViewType type,
			VkFormat format, VkImageAspectFlags imageSapect, uint32_t mipLevels,
			uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);
	};
}  // namespace At0::Ray
