#include "Rpch.h"
#include "RImage2D.h"


namespace At0::Ray
{
	Image2D::Image2D(const UInt2& extent, VkFormat format, VkImageLayout layout,
		VkImageUsageFlags usage, VkFilter filter, VkSamplerAddressMode addressMode,
		VkSampleCountFlagBits samples, bool anisotropic, bool mipmap)
		: Image(filter, addressMode, samples, layout,
			  usage | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
				  VK_IMAGE_USAGE_SAMPLED_BIT,
			  format, 1, 1, { extent.x, extent.y, 1 }),
		  m_Anisotropic(anisotropic), m_Mipmap(mipmap), m_Components(4)
	{
		Image2D::Load();
	}
	void Image2D::Load()
	{
		m_MipLevels = m_Mipmap ? GetMipLevels(m_Extent) : 1;

		CreateImage(m_Image, m_Memory, m_Extent, m_Format, m_Samples, VK_IMAGE_TILING_OPTIMAL,
			m_Usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_MipLevels, m_ArrayLayers,
			VK_IMAGE_TYPE_2D);
		CreateImageSampler(m_Sampler, m_Filter, m_AddressMode, m_Anisotropic, m_MipLevels);
		CreateImageView(m_Image, m_View, VK_IMAGE_VIEW_TYPE_2D, m_Format, VK_IMAGE_ASPECT_COLOR_BIT,
			m_MipLevels, 0, m_ArrayLayers, 0);

		if (m_Mipmap)
		{
			TransitionImageLayout(m_Image, m_Format, VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, m_MipLevels, 0,
				m_ArrayLayers, 0);
			CreateMipmaps(m_Image, m_Extent, m_Format, m_Layout, m_MipLevels, 0, m_ArrayLayers);
		}
		else
		{
			TransitionImageLayout(m_Image, m_Format, VK_IMAGE_LAYOUT_UNDEFINED, m_Layout,
				VK_IMAGE_ASPECT_COLOR_BIT, m_MipLevels, 0, m_ArrayLayers, 0);
		}
	}
}  // namespace At0::Ray
