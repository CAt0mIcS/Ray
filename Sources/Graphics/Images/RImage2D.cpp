#include "Rpch.h"
#include "RImage2D.h"


namespace At0::Ray
{
	Image2D::Image2D(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProps, uint32_t mipLevels, VkImageAspectFlags imageAspect,
		uint32_t arrayLayers, VkImageCreateFlags createFlags)
		: Image(extent, VK_IMAGE_TYPE_2D, format, tiling, usage, memProps, mipLevels, imageAspect,
			  arrayLayers, createFlags)
	{
	}

	void Image2D::Setup(UInt2 extent, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, uint32_t mipLevels,
		VkImageAspectFlags imageAspect, uint32_t arrayLayers, VkImageCreateFlags createFlags)
	{
		m_Extent = extent;
		m_ImageType = VK_IMAGE_TYPE_2D;
		m_Format = format;
		m_Tiling = tiling;
		m_Usage = usage;
		m_MemoryProperties = memProps;
		m_ImageAspect = imageAspect;
		m_MipLevels = mipLevels;
		m_ArrayLayers = arrayLayers;
		m_CreateFlags = createFlags;
		Image::Setup();
	}
}  // namespace At0::Ray
