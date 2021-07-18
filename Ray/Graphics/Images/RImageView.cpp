#include "Rpch.h"
#include "RImageView.h"

#include "Ray/Core/RRendererLoader.h"
#include "Ray/Graphics/RGraphics.h"
#include "Ray/Graphics/Core/RLogicalDevice.h"
#include "RImage.h"

#include "Ray/Utils/RException.h"


namespace At0::Ray
{
	ImageView::ImageView(const Image& image)
	{
		Setup(image, (RrImageViewType)image.GetImageType(), image.GetFormat(), image.GetMipLevels(),
			image.GetAspectFlags(), image.GetArrayLayers());
	}

	ImageView::ImageView(RrImage image, RrImageViewType viewType, RrFormat format,
		uint32_t mipLevels, RrImageAspectFlags aspectFlags, uint32_t layerCount)
	{
		Setup(image, viewType, format, mipLevels, aspectFlags, layerCount);
	}

	ImageView::~ImageView() { RendererAPI::DestroyImageView(Graphics::Get().GetDevice(), m_View); }

	void ImageView::Setup(RrImage image, RrImageViewType viewType, RrFormat format,
		uint32_t mipLevels, RrImageAspectFlags aspectFlags, uint32_t layerCount)
	{
		RrImageViewCreateInfo createInfo{};
		createInfo.flags = 0;
		createInfo.image = image;
		createInfo.viewType = viewType;
		createInfo.format = format;
		createInfo.components = { RrComponentSwizzleIdentity, RrComponentSwizzleIdentity,
			RrComponentSwizzleIdentity, RrComponentSwizzleIdentity };
		createInfo.subresourceRange.aspectMask = aspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = mipLevels;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = layerCount;

		ThrowRenderError(
			RendererAPI::CreateImageView(Graphics::Get().GetDevice(), &createInfo, &m_View),
			"[ImageView] Failed to create");
	}
}  // namespace At0::Ray
