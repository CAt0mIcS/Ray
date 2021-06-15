#include "Rpch.h"
#include "RRenderer.h"

#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "Graphics/Pipelines/Uniforms/RSampler2DUniform.h"

#include "Utils/RException.h"
#include "Utils/RAssert.h"


namespace At0::Ray
{
	Renderer& Renderer::operator=(Renderer&& other) noexcept
	{
		m_DescriptorSets = std::move(other.m_DescriptorSets);
		m_BufferUniforms = std::move(other.m_BufferUniforms);
		m_Sampler2DUniforms = std::move(other.m_Sampler2DUniforms);
		return *this;
	}

	Renderer::Renderer(Renderer&& other) noexcept { *this = std::move(other); }

	Renderer::~Renderer() {}

	Renderer::Renderer(Ref<GraphicsPipeline> pipeline) : m_GraphicsPipeline(std::move(pipeline)) {}

	BufferUniform& Renderer::AddBufferUniform(const std::string& name, ShaderStage stage)
	{
		RAY_MEXPECTS(m_GraphicsPipeline->GetShader().GetReflection(stage).HasUniformBlock(name),
			"[Material] BufferUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		uint32_t set =
			m_GraphicsPipeline->GetShader().GetReflection(stage).GetUniformBlock(name).set;

		// Create descriptor set if the one for this set does not exist yet
		DescriptorSet* pDescriptor = nullptr;
		for (auto& descSet : m_DescriptorSets)
			if (descSet.GetSetNumber() == set)
			{
				pDescriptor = &descSet;
				break;
			}

		// If the descriptor set wasn't found in the existing ones, create it
		if (pDescriptor == nullptr)
			pDescriptor = &m_DescriptorSets.emplace_back(m_GraphicsPipeline->GetDescriptorPool(),
				m_GraphicsPipeline->GetDescriptorSetLayout(set), Pipeline::BindPoint::Graphics,
				m_GraphicsPipeline->GetLayout(), set);

		// Create buffer uniform
		BufferUniform& uniform =
			m_BufferUniforms[set].emplace_back(name, stage, *m_GraphicsPipeline);
		pDescriptor->BindUniform(uniform);

		return uniform;
	}

	Sampler2DUniform& Renderer::AddSampler2DUniform(
		const std::string& name, ShaderStage stage, Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(m_GraphicsPipeline->GetShader().GetReflection(stage).HasUniform(name, true),
			"[Material] Sampler2DUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		uint32_t set = m_GraphicsPipeline->GetShader().GetReflection(stage).GetUniform(name).set;

		// Create descriptor set if the one for this set does not exist yet
		DescriptorSet* pDescriptor = nullptr;
		for (auto& descSet : m_DescriptorSets)
			if (descSet.GetSetNumber() == set)
			{
				pDescriptor = &descSet;
				break;
			}

		// If the descriptor set wasn't found in the existing ones, create it
		if (pDescriptor == nullptr)
			pDescriptor = &m_DescriptorSets.emplace_back(m_GraphicsPipeline->GetDescriptorPool(),
				m_GraphicsPipeline->GetDescriptorSetLayout(set), Pipeline::BindPoint::Graphics,
				m_GraphicsPipeline->GetLayout(), set);

		// Create buffer uniform
		Sampler2DUniform& uniform = m_Sampler2DUniforms[set].emplace_back(
			name, stage, std::move(texture), *m_GraphicsPipeline);
		pDescriptor->BindUniform(uniform);

		return uniform;
	}

	BufferUniform& Renderer::GetBufferUniform(std::string_view name)
	{
		for (auto& descriptors : m_BufferUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff.GetName() == name)
					return uBuff;
		ThrowRuntime("[Renderer] Failed to retrieve BufferUniform \"{0}\"", name);
		return m_BufferUniforms[0][0];
	}

	Sampler2DUniform& Renderer::GetSampler2DUniform(std::string_view name)
	{
		for (auto& descriptors : m_Sampler2DUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff.GetName() == name)
					return uBuff;
		ThrowRuntime("[Renderer] Failed to retrieve Sampler2DUniform \"{0}\"", name);
		return m_Sampler2DUniforms[0][0];
	}
}  // namespace At0::Ray
