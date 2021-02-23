#pragma once

#include "RDynamicBuffer.h"


namespace At0::Ray
{
	class RAY_EXPORT UniformBuffer : public DynamicBuffer
	{
	public:
		UniformBuffer(VkDeviceSize initialSize = 10000000);

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding,
			VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);

	private:
	};
}  // namespace At0::Ray
