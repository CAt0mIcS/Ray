#pragma once

#include "RBuffer.h"

#include <RayRenderer/Pipeline/RDescriptor.h>
#include <RayRenderer/Pipeline/RShader.h>


namespace At0::Ray
{
	class RAY_EXPORT UniformBuffer : public Buffer
	{
	public:
		/**
		 * @param initialSize Initial buffer size. If 0 the value set by
		 * UniformBuffer::SetAllocationSize() will be used (default 2097152MB)
		 */
		UniformBuffer(RrDeviceSize allocSize = 0);
		~UniformBuffer();

		static RrDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding,
			RrDescriptorType descriptorType, RrShaderStageFlags stage, uint32_t count);

		static void SetAllocationSize(RrDeviceSize allocSize) { s_AllocSize = allocSize; }

	private:
		static RrDeviceSize s_AllocSize;
	};
}  // namespace At0::Ray
