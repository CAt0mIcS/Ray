#pragma once

#include "RBuffer.h"


namespace At0::Ray
{
	class RAY_EXPORT UniformBuffer : public Buffer
	{
	public:
		/**
		 * @param initialSize Initial buffer size. If 0 the value set by
		 * UniformBuffer::SetAllocationSize() will be used (default 2097152MB)
		 */
		UniformBuffer(VkDeviceSize allocSize = 0);
		~UniformBuffer();

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding,
			VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);

		static void SetAllocationSize(VkDeviceSize allocSize) { s_AllocSize = allocSize; }

	private:
		static VkDeviceSize s_AllocSize;
	};
}  // namespace At0::Ray
