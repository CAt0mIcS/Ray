#include "Rpch.h"
#include "RImageView.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	ImageView::ImageView(
		VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.image = image;
		createInfo.viewType = viewType;
		createInfo.format = format;
		createInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		createInfo.subresourceRange.aspectMask = aspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		RAY_VK_THROW_FAILED(
			vkCreateImageView(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_View),
			"[ImageView] Failed to create.");
	}

	ImageView::~ImageView() { vkDestroyImageView(Graphics::Get().GetDevice(), m_View, nullptr); }
}  // namespace At0::Ray