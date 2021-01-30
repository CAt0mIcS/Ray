#include "Rpch.h"
#include "RImageDepth.h"

#include "Debug/RException.h"


namespace At0::Ray
{
	static const std::vector<VkFormat> g_TryFormats = { VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM };

	ImageDepth::ImageDepth(const UInt2& extent, VkSampleCountFlagBits samples)
		: Image(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, samples,
			  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			  FindSupportedFormat(g_TryFormats, VK_IMAGE_TILING_OPTIMAL,
				  VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT),
			  1, 1, { extent.x, extent.y, 1 })
	{
		if (m_Format == VK_FORMAT_UNDEFINED)
			RAY_THROW_RUNTIME("[Image[ No depth stencil format could be selected");

		VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (HasStencil(m_Format))
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		CreateImage(m_Image, m_Memory, m_Extent, m_Format, samples, VK_IMAGE_TILING_OPTIMAL,
			m_Usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1, 1, VK_IMAGE_TYPE_2D);
		CreateImageSampler(m_Sampler, m_Filter, m_AddressMode, false, 1);
		CreateImageView(m_Image, m_View, VK_IMAGE_VIEW_TYPE_2D, m_Format, VK_IMAGE_ASPECT_DEPTH_BIT,
			1, 0, 1, 0);
		TransitionImageLayout(
			m_Image, m_Format, VK_IMAGE_LAYOUT_UNDEFINED, m_Layout, aspectMask, 1, 0, 1, 0);
	}
}  // namespace At0::Ray
