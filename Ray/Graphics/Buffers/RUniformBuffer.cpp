#include "Rpch.h"
#include "RUniformBuffer.h"


namespace At0::Ray
{
	VkDeviceSize UniformBuffer::s_AllocSize = 2097152;

	UniformBuffer::UniformBuffer(RrDeviceSize allocSize)
		: Buffer(allocSize ? allocSize != 0 : s_AllocSize, RrBufferUsageUniformBuffer,
			  RrMemoryPropertyDeviceLocal | RrMemoryPropertyHostVisible)
	{
	}

	UniformBuffer::~UniformBuffer() {}

	RrDescriptorSetLayoutBinding UniformBuffer::GetDescriptorSetLayout(
		uint32_t binding, RrDescriptorType descriptorType, RrShaderStageFlags stage, uint32_t count)
	{
		RrDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = stage;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}
}  // namespace At0::Ray
