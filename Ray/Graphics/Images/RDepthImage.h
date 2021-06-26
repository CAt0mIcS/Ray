#pragma once

#include "Ray/RBase.h"
#include "RayBase/RMath.h"
#include "RImage2D.h"


namespace At0::Ray
{
	class DepthImage : public Image2D
	{
	public:
		DepthImage(
			UInt2 extent, VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		static std::vector<VkFormat> FindDepthFormats();
		static bool HasStencilComponent(VkFormat format);
	};
}  // namespace At0::Ray
