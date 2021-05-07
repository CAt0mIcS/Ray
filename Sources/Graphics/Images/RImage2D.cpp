#include "Rpch.h"
#include "RImage2D.h"


namespace At0::Ray
{
	Image2D::Image2D(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProps, uint32_t mipLevels, VkImageAspectFlags imageAspect)
		: Image(extent, VK_IMAGE_TYPE_2D, format, tiling, usage, memProps, mipLevels, imageAspect)
	{
	}

	void Image2D::Setup(UInt2 extent, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, uint32_t mipLevels,
		VkImageAspectFlags imageAspect)
	{
		m_Extent = extent;
		m_ImageType = VK_IMAGE_TYPE_2D;
		m_Format = format;
		m_Tiling = tiling;
		m_Usage = usage;
		m_MemoryProperties = memProps;
		m_ImageAspect = imageAspect;
		m_MipLevels = mipLevels;
		Image::Setup();
	}
}  // namespace At0::Ray
