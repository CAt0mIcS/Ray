#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "Graphics/Pipelines/Uniforms/RSampler2DUniform.h"
#include "Graphics/RCodex.h"

#include "Utils/RAssert.h"


namespace At0::Ray
{
	Material::Material(const Material::Layout& layout)
		: m_DiffuseMap(layout.diffuseMap), m_SpecularMap(layout.specularMap),
		  m_NormalMap(layout.normalMap)
	{
		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(layout);
	}

	Material::~Material() {}

	void Material::CmdBind(const CommandBuffer& cmdBuff) const
	{
		m_GraphicsPipeline->CmdBind(cmdBuff);

		for (const auto& descSet : m_DescriptorSets)
			descSet.second->CmdBind(cmdBuff);
	}

	Ref<BufferUniform> Material::AddBufferUniform(std::string_view name, Shader::Stage stage)
	{
		RAY_MEXPECTS(m_GraphicsPipeline->GetShader().HasUniform(name, stage),
			"[Material] Uniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		uint32_t set = m_GraphicsPipeline->GetShader().GetUniformBlocks(stage)->Get(name)->set;

		// Create descriptor set if the one for this set does not exist yet
		if (!m_DescriptorSets[set])
			m_DescriptorSets[set] = MakeRef<DescriptorSet>(m_GraphicsPipeline->GetDescriptorPool(),
				m_GraphicsPipeline->GetDescriptorSetLayout(set), Pipeline::BindPoint::Graphics,
				m_GraphicsPipeline->GetLayout(), set);

		// Create buffer uniform
		Ref<BufferUniform> uniform = m_BufferUniforms[set].emplace_back(
			MakeRef<BufferUniform>(name, stage, *m_GraphicsPipeline));
		m_DescriptorSets[set]->BindUniform(*uniform);

		return uniform;
	}

	Ref<Sampler2DUniform> Material::AddSampler2DUniform(
		std::string_view name, Shader::Stage stage, Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(m_GraphicsPipeline->GetShader().HasUniform(name, stage),
			"[Material] Uniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		uint32_t set = m_GraphicsPipeline->GetShader().GetUniforms(stage)->Get(name)->set;

		// Create descriptor set if the one for this set does not exist yet
		if (!m_DescriptorSets[set])
			m_DescriptorSets[set] = MakeRef<DescriptorSet>(m_GraphicsPipeline->GetDescriptorPool(),
				m_GraphicsPipeline->GetDescriptorSetLayout(set), Pipeline::BindPoint::Graphics,
				m_GraphicsPipeline->GetLayout(), set);

		// Create buffer uniform
		Ref<Sampler2DUniform> uniform = m_Sampler2DUniforms[set].emplace_back(
			MakeRef<Sampler2DUniform>(name, stage, std::move(texture), *m_GraphicsPipeline));
		m_DescriptorSets[set]->BindUniform(*uniform);

		return uniform;
	}

	Ref<BufferUniform> Material::GetBufferUniform(std::string_view name) const
	{
		for (auto& descriptors : m_BufferUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff->GetName() == name)
					return uBuff;
		return nullptr;
	}

	Ref<Sampler2DUniform> Material::GetSampler2DUniform(std::string_view name) const
	{
		for (auto& descriptors : m_Sampler2DUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff->GetName() == name)
					return uBuff;
		return nullptr;
	}

	Material& Material::operator=(Material&& other) noexcept
	{
		m_GraphicsPipeline = std::move(other.m_GraphicsPipeline);
		m_DescriptorSets = std::move(other.m_DescriptorSets);
		m_BufferUniforms = std::move(other.m_BufferUniforms);
		m_Sampler2DUniforms = std::move(other.m_Sampler2DUniforms);
		return *this;
	}

	Material::Material(Material&& other) noexcept { *this = std::move(other); }
}  // namespace At0::Ray
