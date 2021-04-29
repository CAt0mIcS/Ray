#include "Rpch.h"
#include "RBufferUniform.h"

#include "Graphics/RGraphics.h"
#include "Utils/RString.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"


namespace At0::Ray
{
	BufferUniform::BufferUniform(std::string_view name, VkDescriptorSetLayout descSetLayout,
		VkDescriptorPool descSetPool, Pipeline::BindPoint bindPoint,
		VkPipelineLayout pipelineLayout, uint32_t bufferSize, uint32_t set, uint32_t binding)
		: Uniform(name), m_BufferSize(bufferSize), m_Binding(binding)
	{
		m_DescriptorSet =
			MakeScope<DescriptorSet>(descSetPool, descSetLayout, bindPoint, pipelineLayout, set);

		Setup(bufferSize, binding);
	}

	BufferUniform::BufferUniform(std::string_view name, const Pipeline& pipeline,
		uint32_t bufferSize, uint32_t set, uint32_t binding)
		: Uniform(name), m_BufferSize(bufferSize), m_Binding(binding)
	{
		m_DescriptorSet = MakeScope<DescriptorSet>(pipeline.GetDescriptorPool(),
			pipeline.GetDescriptorSetLayout(set), pipeline.GetBindPoint(), pipeline.GetLayout(),
			set);
		Setup(bufferSize, binding);
	}

	BufferUniform::BufferUniform(
		std::string_view uniformBlockName, Shader::Stage stage, const Pipeline& pipeline)
		: Uniform(uniformBlockName)
	{
		uint32_t size = 0;
		uint32_t binding = 0;
		uint32_t set = 0;
		if (auto uniformBlocks = pipeline.GetShader().GetUniformBlocks(stage))
		{
			if (auto uBlockData = uniformBlocks->Get(uniformBlockName))
			{
				size = uBlockData->size;
				binding = uBlockData->binding;
				set = uBlockData->set;

				m_Uniforms = uBlockData->uniforms;
			}
		}

		// The size should never be 0 here
		RAY_MEXPECTS(size != 0, "[BufferUniform] Failed to find uniform {0} in stage {1}",
			uniformBlockName, String::Construct(stage));

		m_BufferSize = size;
		m_Binding = binding;

		m_DescriptorSet = MakeScope<DescriptorSet>(pipeline.GetDescriptorPool(),
			pipeline.GetDescriptorSetLayout(set), pipeline.GetBindPoint(), pipeline.GetLayout(),
			set);

		Setup(size, binding);
	}

	BufferUniform::~BufferUniform() {}

	void BufferUniform::Update(void* data, uint32_t size)
	{
		BufferSynchronizer::Get().Update(data, size, m_GlobalBufferOffset);
	}

	BufferUniform& BufferUniform::operator=(const BufferUniform& other)
	{
		Uniform::operator=(other);
		m_BufferSize = other.m_BufferSize;
		m_Binding = other.m_Binding;

		Setup(m_BufferSize, m_Binding);
		BufferSynchronizer::Get().Copy(
			other.m_GlobalBufferOffset, m_GlobalBufferOffset, m_BufferSize);

		return *this;
	}

	BufferUniform::BufferUniform(const BufferUniform& other) : Uniform(other.m_Name)
	{
		*this = other;
	}

	void BufferUniform::Setup(uint32_t bufferSize, uint32_t binding)
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

		std::vector<VkWriteDescriptorSet> descWrites{};
		VkWriteDescriptorSet descWrite{};
		descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite.dstSet = *m_DescriptorSet;
		descWrite.dstBinding = binding;
		descWrite.dstArrayElement = 0;
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWrite.pImageInfo = nullptr;
		descWrite.pBufferInfo = &bufferInfo;
		descWrite.pTexelBufferView = nullptr;
		descWrites.emplace_back(descWrite);

		DescriptorSet::Update(descWrites);
	}

	uint32_t BufferUniform::GetUniformOffsetInBlock(std::string_view uniformName) const
	{
		if (!m_Uniforms)
			return 0;

		return m_Uniforms->Get(uniformName)->offset;
	}
}  // namespace At0::Ray
