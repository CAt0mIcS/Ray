#include "Rpch.h"
#include "RUniformBuffer.h"


namespace At0::Ray
{
	VkDeviceSize UniformBuffer::s_AllocSize = 2097152;

	UniformBuffer::UniformBuffer(VkDeviceSize allocSize)
		: Buffer(allocSize ? allocSize != 0 : s_AllocSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
	{
	}

	UniformBuffer::~UniformBuffer() {}

	VkDescriptorSetLayoutBinding UniformBuffer::GetDescriptorSetLayout(
		uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = stage;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}
}  // namespace At0::Ray
