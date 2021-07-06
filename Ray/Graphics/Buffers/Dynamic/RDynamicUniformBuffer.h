#pragma once

#include "RDynamicBuffer.h"
#include "../RUniformBuffer.h"

#include <atomic>


namespace At0::Ray
{
	class RAY_EXPORT DynamicUniformBuffer : public DynamicBuffer
	{
	public:
		DynamicUniformBuffer(RrDeviceSize size);

		static void Reset();

	private:
		static Ref<UniformBuffer> s_UniformBuffer;
		static std::atomic<RrDeviceSize> s_NextOffset;
	};
}  // namespace At0::Ray
