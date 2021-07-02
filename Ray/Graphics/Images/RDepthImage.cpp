#include "Rpch.h"
#include "RDepthImage.h"


namespace At0::Ray
{
	DepthImage::DepthImage(UInt2 extent, RrImageUsageFlags usage)
		: Image2D(extent, FindDepthFormats()[0], RrImageTilingOptimal, usage,
			  RrMemoryPropertyDeviceLocal, 1, RrImageAspectDepth)
	{
	}

	std::vector<RrFormat> DepthImage::FindDepthFormats()
	{
		return FindSupportedFormats(
			{ RRFORMAT_D32_SFLOAT, RRFORMAT_D32_SFLOAT_S8_UINT, RRFORMAT_D24_UNORM_S8_UINT },
			RrImageTilingOptimal, RrFormatFeatureDepthStencilAttachment);
	}

	bool DepthImage::HasStencilComponent(RrFormat format)
	{
		return format == RRFORMAT_D32_SFLOAT_S8_UINT || format == RRFORMAT_D24_UNORM_S8_UINT;
	}
}  // namespace At0::Ray
