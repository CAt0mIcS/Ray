#pragma once

#include "../RBase.h"
#include "RImage.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT ImageDepth : public Image
	{
	public:
		ImageDepth(const UInt2& extent, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
	};
}  // namespace At0::Ray
