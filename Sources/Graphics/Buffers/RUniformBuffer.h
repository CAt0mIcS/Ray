#pragma once

#include "RDynamicBuffer.h"


namespace At0::Ray
{
	class RAY_EXPORT UniformBuffer : public DynamicBuffer
	{
		inline static uint32_t s_InitialSize = 1000;

	public:
		UniformBuffer();

	private:
	};
}  // namespace At0::Ray
