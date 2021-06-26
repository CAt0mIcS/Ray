#include "Rpch.h"
#include "RRenderer.h"

#include "Shading/RMaterial.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "Graphics/Pipelines/Uniforms/RSampler2DUniform.h"

#include "RayBase/RException.h"
#include "RayBase/RAssert.h"


namespace At0::Ray
{
	Renderer& Renderer::operator=(Renderer&& other) noexcept
	{
		m_DescriptorSets = std::move(other.m_DescriptorSets);
		m_BufferUniforms = std::move(other.m_BufferUniforms);
		m_Sampler2DUniforms = std::move(other.m_Sampler2DUniforms);
		m_Material = std::move(other.m_Material);
		return *this;
	}

	Renderer::Renderer(Renderer&& other) noexcept { *this = std::move(other); }

	Renderer::~Renderer() {}

	Renderer::Renderer(Ref<Material> material, bool automaticUniformEmplacement)
		: m_Material(material)
	{
		AddBufferUniform("PerObjectData", ShaderStage::Vertex);
		if (automaticUniformEmplacement)
			AddUniforms();
	}

	BufferUniform& Renderer::AddBufferUniform(const std::string& name, ShaderStage stage)
	{
		RAY_MEXPECTS(
			m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage).HasUniformBlock(
				name),
			"[Renderer] BufferUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct((VkShaderStageFlags)stage));

		uint32_t set = m_Material->GetGraphicsPipeline()
						   .GetShader()
						   .GetReflection(stage)
						   .GetUniformBlock(name)
						   .set;

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
			pDescriptor = &m_DescriptorSets.emplace_back(
				m_Material->GetGraphicsPipeline().GetDescriptorPool(),
				m_Material->GetGraphicsPipeline().GetDescriptorSetLayout(set),
				Pipeline::BindPoint::Graphics, m_Material->GetGraphicsPipeline().GetLayout(), set);

		RAY_MEXPECTS(std::find_if(m_BufferUniforms[set].begin(), m_BufferUniforms[set].end(),
						 [&name](const auto& uniform) { return uniform.GetName() == name; }) ==
						 m_BufferUniforms[set].end(),
			"[Renderer] BufferUniform \"{0}\" already added", name);

		// Create buffer uniform
		BufferUniform& uniform =
			m_BufferUniforms[set].emplace_back(name, stage, m_Material->GetGraphicsPipeline());
		pDescriptor->BindUniform(uniform);

		return uniform;
	}

	Sampler2DUniform& Renderer::AddSampler2DUniform(
		const std::string& name, ShaderStage stage, Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage).HasUniform(
						 name, true),
			"[Renderer] Sampler2DUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct((VkShaderStageFlags)stage));

		uint32_t set =
			m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage).GetUniform(name).set;

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
			pDescriptor = &m_DescriptorSets.emplace_back(
				m_Material->GetGraphicsPipeline().GetDescriptorPool(),
				m_Material->GetGraphicsPipeline().GetDescriptorSetLayout(set),
				Pipeline::BindPoint::Graphics, m_Material->GetGraphicsPipeline().GetLayout(), set);

		RAY_MEXPECTS(std::find_if(m_Sampler2DUniforms[set].begin(), m_Sampler2DUniforms[set].end(),
						 [&name](const auto& uniform) { return uniform.GetName() == name; }) ==
						 m_Sampler2DUniforms[set].end(),
			"[Renderer] Sampler2DUniform \"{0}\" already added", name);

		// Create buffer uniform
		Sampler2DUniform& uniform = m_Sampler2DUniforms[set].emplace_back(
			name, stage, std::move(texture), m_Material->GetGraphicsPipeline());
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

	DescriptorSet& Renderer::GetDescriptorSet(std::string_view uniformName)
	{
		for (const auto& [set, uniforms] : m_BufferUniforms)
			for (const auto& uniform : uniforms)
				if (uniform.GetName() == uniformName)
					for (auto& descSet : m_DescriptorSets)
						if (descSet.GetSetNumber() == set)
							return descSet;

		for (const auto& [set, uniforms] : m_Sampler2DUniforms)
			for (const auto& uniform : uniforms)
				if (uniform.GetName() == uniformName)
					for (auto& descSet : m_DescriptorSets)
						if (descSet.GetSetNumber() == set)
							return descSet;
		ThrowRuntime("[Renderer] Failed to retrieve Descriptor Set of uniform with name \"{0}\"",
			uniformName);
		return m_DescriptorSets[0];
	}

	void Renderer::AddUniforms()
	{
		for (const auto& [stage, reflection] :
			m_Material->GetGraphicsPipeline().GetShader().GetReflections())
		{
			for (const auto& uBlock : reflection.GetUniformBlocks())
			{
				if (uBlock.name == UniformTag::Shading)
					AddBufferUniform(UniformTag::Shading, stage);
			}

			// Only sampler uniforms can be outside of a block
			for (const auto& uniform : reflection.GetUniforms())
			{
				if (uniform.name == UniformTag::AlbedoMapSampler)
					AddSampler2DUniform(
						UniformTag::AlbedoMapSampler, stage, m_Material->GetAlbedoMap());
				else if (uniform.name == UniformTag::DiffuseMapSampler)
					AddSampler2DUniform(
						UniformTag::DiffuseMapSampler, stage, m_Material->GetDiffuseMap());
				else if (uniform.name == UniformTag::SpecularMapSampler)
					AddSampler2DUniform(
						UniformTag::SpecularMapSampler, stage, m_Material->GetSpecularMap());
				else if (uniform.name == UniformTag::NormalMapSampler)
					AddSampler2DUniform(
						UniformTag::NormalMapSampler, stage, m_Material->GetNormalMap());
			}
		}
	}
}  // namespace At0::Ray
