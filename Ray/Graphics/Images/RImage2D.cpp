#include "Rpch.h"
#include "RImage2D.h"


namespace At0::Ray
{
	Ref<Image2D> Image2D::Acquire(UInt2 extent, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, uint32_t mipLevels,
		VkImageAspectFlags imageAspect, uint32_t arrayLayers, VkImageCreateFlags createFlags)
	{
		return Resources::Get().EmplaceIfNonExistent<Image2D>(
			String::Serialize("Image2D{0}{1}{2}{3}{4}{5}{6}{7}{8}{9}", extent.x, extent.y,
				(uint32_t)format, (uint32_t)tiling, (uint32_t)usage, (uint32_t)memProps, mipLevels,
				(uint32_t)imageAspect, arrayLayers, (uint32_t)createFlags),
			std::move(extent), format, tiling, usage, memProps, mipLevels, imageAspect, arrayLayers,
			createFlags);
	}

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
