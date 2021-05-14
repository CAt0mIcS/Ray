#include "Rpch.h"
#include "RMeshRenderer.h"

#include "RTransform.h"

#include "Shading/RMaterial.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Images/RTexture2D.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	MeshRenderer::MeshRenderer(Entity entity, Ref<Material> material)
		: Component(entity), m_Material(std::move(material))
	{
	}

	void MeshRenderer::Render(const CommandBuffer& cmdBuff) const { m_Material->CmdBind(cmdBuff); }

	void MeshRenderer::Update()
	{
		GetBufferUniform(UniformTag::PerObjectData)["Model"] =
			GetEntity().Get<Transform>().AsMatrix();
	}

	BufferUniform& MeshRenderer::AddBufferUniform(std::string_view name, Shader::Stage stage)
	{
		RAY_MEXPECTS(m_Material->GetGraphicsPipeline().GetShader().HasUniform(name, stage),
			"[MeshRenderer] Uniform \"{0}\" not found in stage \"{1}\"", name,
			String::Construct(stage));

		auto* uBlock =
			m_Material->GetGraphicsPipeline().GetShader().GetUniformBlocks(stage)->Get(name);

		// Share descriptor if it has the same set number and the descriptor set already exists
		for (const Scope<DescriptorSet>& descSet : m_DescriptorSets)
		{
			if (descSet->GetSetNumber() == uBlock->set)
			{
				m_BufferUniforms[descSet->GetSetNumber()] =
					MakeScope<BufferUniform>(name, stage, m_Material->GetGraphicsPipeline());
				return *m_BufferUniforms[descSet->GetSetNumber()];
			}
		}

		// Create if a descriptor set with the set number of the uniform doesn't exist
		const Scope<DescriptorSet>& descSet = m_DescriptorSets.emplace_back(
			MakeScope<DescriptorSet>(m_Material->GetGraphicsPipeline().GetDescriptorPool(),
				m_Material->GetGraphicsPipeline().GetDescriptorSetLayout(uBlock->set),
				Pipeline::BindPoint::Graphics, m_Material->GetGraphicsPipeline().GetLayout(),
				uBlock->set));
		m_BufferUniforms[descSet->GetSetNumber()] =
			MakeScope<BufferUniform>(name, stage, m_Material->GetGraphicsPipeline());

		return *m_BufferUniforms[descSet->GetSetNumber()];
	}

	Sampler2DUniform& MeshRenderer::AddSampler2DUniform(
		std::string_view name, Shader::Stage stage, Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(m_Material->GetGraphicsPipeline().GetShader().HasUniform(name, stage),
			"[MeshRenderer] Uniform \"{0}\" not found in stage \"{1}\"", name,
			String::Construct(stage));

		auto* uniform = m_Material->GetGraphicsPipeline().GetShader().GetUniforms(stage)->Get(name);

		// Share descriptor if it has the same set number and the descriptor set already exists
		for (const Scope<DescriptorSet>& descSet : m_DescriptorSets)
		{
			if (descSet->GetSetNumber() == uniform->set)
			{
				m_Sampler2DUniforms[descSet->GetSetNumber()] = MakeScope<Sampler2DUniform>(
					name, stage, std::move(texture), m_Material->GetGraphicsPipeline());
				return *m_Sampler2DUniforms[descSet->GetSetNumber()];
			}
		}

		// Create if a descriptor set with the set number of the uniform doesn't exist
		const Scope<DescriptorSet>& descSet = m_DescriptorSets.emplace_back(
			MakeScope<DescriptorSet>(m_Material->GetGraphicsPipeline().GetDescriptorPool(),
				m_Material->GetGraphicsPipeline().GetDescriptorSetLayout(uniform->set),
				Pipeline::BindPoint::Graphics, m_Material->GetGraphicsPipeline().GetLayout(),
				uniform->set));
		m_Sampler2DUniforms[descSet->GetSetNumber()] = MakeScope<Sampler2DUniform>(
			name, stage, std::move(texture), m_Material->GetGraphicsPipeline());

		return *m_Sampler2DUniforms[descSet->GetSetNumber()];
	}

	BufferUniform& MeshRenderer::GetBufferUniform(std::string_view name)
	{
		for (const auto& [set, uniform] : m_BufferUniforms)
			if (uniform->GetName() == name)
				return *uniform;

		RAY_THROW_RUNTIME("[MeshRenderer] Failed to retrieve BufferUniform \"{0}\"", name);
	}

	Sampler2DUniform& MeshRenderer::GetSampler2DUniform(std::string_view name)
	{
		for (const auto& [set, uniform] : m_Sampler2DUniforms)
			if (uniform->GetName() == name)
				return *uniform;

		RAY_THROW_RUNTIME("[MeshRenderer] Failed to retrieve Sampler2DUniform \"{0}\"", name);
	}
}  // namespace At0::Ray
