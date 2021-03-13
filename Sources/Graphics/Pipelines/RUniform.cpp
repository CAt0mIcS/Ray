#include "Rpch.h"
#include "RUniform.h"

#include "Graphics/RGraphics.h"
#include "Utils/RString.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"


namespace At0::Ray
{
	Uniform::~Uniform() {}

	void Uniform::CmdBind(const CommandBuffer& cmdBuff) const { m_DescriptorSet->CmdBind(cmdBuff); }

	Uniform& Uniform::operator=(Uniform&& other) noexcept
	{
		m_DescriptorSet = std::move(other.m_DescriptorSet);
		return *this;
	}

	Uniform::Uniform(Uniform&& other) noexcept { *this = std::move(other); }

	BufferUniform::BufferUniform(VkDescriptorSetLayout descSetLayout, VkDescriptorPool descSetPool,
		Pipeline::BindPoint bindPoint, VkPipelineLayout pipelineLayout, uint32_t bufferSize,
		uint32_t set, uint32_t binding)
	{
		m_DescriptorSet =
			MakeScope<DescriptorSet>(descSetPool, descSetLayout, bindPoint, pipelineLayout, set);

		Setup(bufferSize, binding);
	}

	BufferUniform::BufferUniform(
		const Pipeline& pipeline, uint32_t bufferSize, uint32_t set, uint32_t binding)
	{
		m_DescriptorSet = MakeScope<DescriptorSet>(pipeline.GetDescriptorPool(),
			pipeline.GetDescriptorSetLayout(set), pipeline.GetBindPoint(), pipeline.GetLayout(),
			set);
		Setup(bufferSize, binding);
	}

	BufferUniform::BufferUniform(
		std::string_view uniformName, Shader::Stage stage, const Pipeline& pipeline)
	{
		uint32_t size = 0;
		uint32_t binding = 0;
		uint32_t set = 0;
		if (auto uniforms = pipeline.GetShader().GetUniforms(stage))
		{
			if (auto uniformData = uniforms->Get(uniformName))
			{
				size = uniformData->size;
				binding = uniformData->binding;
				set = uniformData->set;
			}
		}
		if (auto uniformBlocks = pipeline.GetShader().GetUniformBlocks(stage))
		{
			if (auto uBlockData = uniformBlocks->Get(uniformName))
			{
				size = uBlockData->size;
				binding = uBlockData->binding;
				set = uBlockData->set;
			}
		}

		// The size should never be 0 here
		RAY_MEXPECTS(size != 0, "[BufferUniform] Failed to find uniform {0} in stage {1}",
			uniformName, String::Construct(stage));

		m_DescriptorSet = MakeScope<DescriptorSet>(pipeline.GetDescriptorPool(),
			pipeline.GetDescriptorSetLayout(set), pipeline.GetBindPoint(), pipeline.GetLayout(),
			set);

		Setup(size, binding);
	}

	BufferUniform::~BufferUniform() {}

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

	ImageSamplerUniform::~ImageSamplerUniform() {}
}  // namespace At0::Ray
