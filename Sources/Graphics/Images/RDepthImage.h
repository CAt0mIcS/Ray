#pragma once

#include "../../RBase.h"
#include "../../Core/RMath.h"
#include "RImage2D.h"


namespace At0::Ray
{
	class DepthImage : public Image2D
	{
	public:
		DepthImage(UInt2 extent);

		static std::vector<VkFormat> FindDepthFormats();
		static bool HasStencilComponent(VkFormat format);
	};
}  // namespace At0::Ray
