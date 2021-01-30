#pragma once

#include "../RBase.h"

struct VkImage_T;
struct VkImageView_T;
enum VkImageViewType;
enum VkFormat;

namespace At0::Ray
{
	class RAY_EXPORT Image
	{
	public:
		static void CreateImageView(const VkImage_T* image, VkImageView_T*& imageView,
			VkImageViewType type, VkFormat format, uint32_t imageAspect, uint32_t mipLevels,
			uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);
	};
}  // namespace At0::Ray
