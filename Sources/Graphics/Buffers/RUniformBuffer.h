#pragma once

#include "RDynamicBuffer.h"


namespace At0::Ray
{
	class RAY_EXPORT UniformBuffer : public DynamicBuffer
	{
	public:
		/**
		 * @param initialSize Initial buffer size (default 2MB)
		 */
		UniformBuffer(VkDeviceSize initialSize = 2097152);
		~UniformBuffer();

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding,
			VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);

	private:
	};
}  // namespace At0::Ray
