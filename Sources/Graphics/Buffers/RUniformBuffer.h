#pragma once

#include "RDynamicBuffer.h"


namespace At0::Ray
{
	class RAY_EXPORT UniformBuffer : public DynamicBuffer
	{
	public:
		UniformBuffer(VkDeviceSize initialSize = 10000);

	private:
	};
}  // namespace At0::Ray
