#include "RDepthImage.h"

#include "Graphics/Core/RRenderContext.h"

namespace At0::Ray
{
	DepthImage::DepthImage(RenderContext& context, Ref<CommandPool> transientCommandPool, UInt2 extent,
		VkImageUsageFlags usage)
		: Image(context, std::move(transientCommandPool), extent, VK_IMAGE_TYPE_2D,
			  FindDepthFormats(context.physicalDevice)[0], VK_IMAGE_TILING_OPTIMAL, usage,
			  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1, VK_IMAGE_ASPECT_DEPTH_BIT)
	{
	}

	std::vector<VkFormat> DepthImage::FindDepthFormats(const PhysicalDevice& physicalDevice)
	{
		return FindSupportedFormats(physicalDevice,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	bool DepthImage::HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
}  // namespace At0::Ray
