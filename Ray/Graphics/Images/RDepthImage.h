﻿#pragma once

#include "../../RBase.h"
#include "../../Core/RMath.h"
#include "RImage.h"


namespace At0::Ray
{
	class DepthImage : public Image
	{
	public:
		DepthImage(
			UInt2 extent, VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		static std::vector<VkFormat> FindDepthFormats();
		static bool HasStencilComponent(VkFormat format);
	};
}  // namespace At0::Ray
