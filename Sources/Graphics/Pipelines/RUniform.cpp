#include "Rpch.h"
#include "RUniform.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"


namespace At0::Ray
{
	Uniform::Uniform(VkDescriptorSetLayout descSetLayout, VkDescriptorPool descSetPool,
		Pipeline::BindPoint bindPoint, VkPipelineLayout pipelineLayout, uint32_t bufferSize,
		uint32_t binding, uint32_t set, VkDescriptorType descType)
		: m_DescriptorSet(descSetPool, descSetLayout, bindPoint, pipelineLayout, set)
	{
		Setup(bufferSize, binding, descType);
	}

	Uniform::Uniform(const Pipeline& pipeline, uint32_t bufferSize, uint32_t binding, uint32_t set,
		VkDescriptorType descType)
		: m_DescriptorSet(pipeline.GetDescriptorPool(), pipeline.GetDescriptorSetLayouts()[set],
			  pipeline.GetBindPoint(), pipeline.GetLayout(), set)
	{
		Setup(bufferSize, binding, descType);
	}

	void Uniform::CmdBind(const CommandBuffer& cmdBuff) const { m_DescriptorSet.CmdBind(cmdBuff); }

	void Uniform::Setup(uint32_t bufferSize, uint32_t binding, VkDescriptorType descType)
	{
		uint32_t minBufferAlignment = Graphics::Get()
										  .GetPhysicalDevice()
										  .GetProperties()
										  .limits.minUniformBufferOffsetAlignment;
		BufferSynchronizer::Get().Emplace(bufferSize, minBufferAlignment, &m_GlobalBufferOffset);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer =
			BufferSynchronizer::Get().GetUniformBuffer().GetBuffer(m_GlobalBufferOffset);
		bufferInfo.offset =
			BufferSynchronizer::Get().GetUniformBuffer().GetOffset(m_GlobalBufferOffset);
		bufferInfo.range = bufferSize;

		VkWriteDescriptorSet descWrite{};
		descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite.dstSet = m_DescriptorSet;
		descWrite.dstBinding = binding;
		descWrite.dstArrayElement = 0;
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = descType;
		descWrite.pImageInfo = nullptr;
		descWrite.pBufferInfo = &bufferInfo;
		descWrite.pTexelBufferView = nullptr;

		DescriptorSet::Update({ descWrite });
	}
}  // namespace At0::Ray
