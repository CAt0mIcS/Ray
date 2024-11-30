#include "RImageView.h"

#include "Graphics/Core/RRenderContext.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "RImage.h"


namespace At0::Ray
{
	ImageView::ImageView(const Image& image)
		: ImageView(image.GetRenderContext(), image, (VkImageViewType)image.GetImageType(),
			  image.GetFormat(), image.GetMipLevels(), image.GetAspectFlags(),
			  image.GetArrayLayers())
	{
	}

	ImageView::ImageView(const RenderContext& context, VkImage image, VkImageViewType viewType,
		VkFormat format, uint32_t mipLevels, VkImageAspectFlags aspectFlags, uint32_t layerCount)
		: m_Context(context)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.image = image;
		createInfo.viewType = viewType;
		createInfo.format = format;
		createInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		createInfo.subresourceRange = {};
		createInfo.subresourceRange.aspectMask = aspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = mipLevels;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = layerCount;

		ThrowVulkanError(vkCreateImageView(m_Context.device, &createInfo, nullptr, &m_View),
			"[ImageView] Failed to create");
	}

	ImageView::~ImageView()
	{
		vkDestroyImageView(m_Context.device, m_View, nullptr);
	}
}  // namespace At0::Ray
