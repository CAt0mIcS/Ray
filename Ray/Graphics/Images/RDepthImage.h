#pragma once

#include "Ray/RBase.h"
#include "Ray/Core/RMath.h"
#include "RImage2D.h"


namespace At0::Ray
{
	class DepthImage : public Image2D
	{
	public:
		DepthImage(UInt2 extent, RrImageUsageFlags usage = RrImageUsageDepthStencilAttachment);

		static std::vector<RrFormat> FindDepthFormats();
		static bool HasStencilComponent(RrFormat format);
	};
}  // namespace At0::Ray
