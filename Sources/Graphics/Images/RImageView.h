#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class ImageView
	{
	public:
		ImageView(VkImage image, VkFormat format,
			VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT,
			VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);
		~ImageView();

		operator const VkImageView&() const { return m_View; }

	private:
		VkImageView m_View;
	};
}  // namespace At0::Ray
