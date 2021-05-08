#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class Image;

	class ImageView : NonCopyable
	{
	public:
		ImageView(const Image& image);
		ImageView(VkImage image, VkImageViewType viewType, VkFormat format, uint32_t mipLevels = 1,
			VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, uint32_t layerCount = 1);
		~ImageView();

		operator const VkImageView&() const { return m_View; }

	private:
		void Setup(VkImage image, VkImageViewType viewType, VkFormat format, uint32_t mipLevels,
			VkImageAspectFlags aspectFlags, uint32_t layerCount);

	private:
		VkImageView m_View = VK_NULL_HANDLE;
	};
}  // namespace At0::Ray
