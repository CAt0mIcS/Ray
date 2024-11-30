#pragma once

#include "../../RBase.h"
#include "../../Core/RMath.h"
#include "RImage.h"


namespace At0::Ray
{
	class RenderContext;
	class CommandPool;
	class PhysicalDevice;

	class DepthImage : public Image
	{
	public:
		DepthImage(RenderContext& context, Ref<CommandPool> transientCommandPool, UInt2 extent,
			VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		static std::vector<VkFormat> FindDepthFormats(const PhysicalDevice& context);
		static bool HasStencilComponent(VkFormat format);
	};
}  // namespace At0::Ray
