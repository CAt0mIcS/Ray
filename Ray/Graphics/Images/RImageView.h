#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class Image;
	class RenderContext;

	class ImageView : NonCopyable
	{
	public:
		ImageView(const Image& image);
		ImageView(const RenderContext& context, VkImage image, VkImageViewType viewType,
			VkFormat format, uint32_t mipLevels = 1,
			VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, uint32_t layerCount = 1);
		~ImageView();

		operator const VkImageView&() const { return m_View; }

	private:
		VkImageView m_View = VK_NULL_HANDLE;
		const RenderContext& m_Context;
	};
}  // namespace At0::Ray
