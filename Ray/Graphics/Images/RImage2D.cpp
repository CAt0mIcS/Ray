#include "Rpch.h"
#include "RImage2D.h"

#include "Ray/Utils/RString.h"


namespace At0::Ray
{
	Ref<Image2D> Image2D::Acquire(UInt2 extent, RrFormat format, RrImageTiling tiling,
		RrImageUsageFlags usage, RrMemoryPropertyFlags memProps, uint32_t mipLevels,
		RrImageAspectFlags imageAspect, uint32_t arrayLayers, RrImageCreateFlags createFlags)
	{
		return Resources::Get().EmplaceIfNonExistent<Image2D>(
			String::Serialize("Image2D{0}{1}{2}{3}{4}{5}{6}{7}{8}{9}", extent.x, extent.y,
				(uint32_t)format, (uint32_t)tiling, (uint32_t)usage, (uint32_t)memProps, mipLevels,
				(uint32_t)imageAspect, arrayLayers, (uint32_t)createFlags),
			std::move(extent), format, tiling, usage, memProps, mipLevels, imageAspect, arrayLayers,
			createFlags);
	}

	Image2D::Image2D(UInt2 extent, RrFormat format, RrImageTiling tiling, RrImageUsageFlags usage,
		RrMemoryPropertyFlags memProps, uint32_t mipLevels, RrImageAspectFlags imageAspect,
		uint32_t arrayLayers, RrImageCreateFlags createFlags)
		: Image(extent, RrImageType2D, format, tiling, usage, memProps, mipLevels, imageAspect,
			  arrayLayers, createFlags)
	{
	}

	void Image2D::Setup(UInt2 extent, RrFormat format, RrImageTiling tiling,
		RrImageUsageFlags usage, RrMemoryPropertyFlags memProps, uint32_t mipLevels,
		RrImageAspectFlags imageAspect, uint32_t arrayLayers, RrImageCreateFlags createFlags)
	{
		m_Extent = extent;
		m_ImageType = RrImageType2D;
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
