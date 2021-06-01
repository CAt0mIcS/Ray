#include "Rpch.h"
#include "RMeshRenderer.h"

#include "RTransform.h"
#include "RMesh.h"

#include "Shading/RMaterial.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Images/RTexture2D.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	MeshRenderer::MeshRenderer(
		Entity entity, Ref<Material> material, bool automaticUniformEmplacement)
		: Component(entity), m_Material(std::move(material))
	{
		if (automaticUniformEmplacement)
			AddUniforms();
	}

	void MeshRenderer::Render(const CommandBuffer& cmdBuff) const
	{
		m_Material->CmdBind(cmdBuff);

		for (const auto& descSet : m_DescriptorSets)
			descSet.CmdBind(cmdBuff);

		// RAY_TODO: MeshRenderer requires mesh!
		GetEntity().Get<Mesh>().CmdBind(cmdBuff);
	}

	void MeshRenderer::Update()
	{
		RAY_MEXPECTS(m_PerObjectDataUniformRef,
			"[MeshRenderer] Mandatory BufferUniform \"{0}\" was not added",
			UniformTag::PerObjectData);

		if (auto& tform = GetEntity().Get<Transform>(); tform.HasChanged())
		{
			if (GetEntity().HasParent())
				(*m_PerObjectDataUniformRef) =
					GetEntity().GetParent().Get<Transform>().AsMatrix() * tform.AsMatrix();
			else
				(*m_PerObjectDataUniformRef) = tform.AsMatrix();
		}
	}

	BufferUniform& MeshRenderer::AddBufferUniform(std::string_view name, Shader::Stage stage)
	{
		RAY_MEXPECTS(m_Material->GetGraphicsPipeline().GetShader().HasUniform(name, stage),
			"[Material] BufferUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		uint32_t set =
			m_Material->GetGraphicsPipeline().GetShader().GetUniformBlocks(stage)->Get(name)->set;

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

		// Create buffer uniform
		Scope<BufferUniform>& uniform = m_BufferUniforms[set].emplace_back(
			MakeScope<BufferUniform>(name, stage, m_Material->GetGraphicsPipeline()));
		pDescriptor->BindUniform(*uniform);

		if (name == UniformTag::PerObjectData)
			m_PerObjectDataUniformRef = (*uniform)["Model"];

		return *uniform;
	}

	Sampler2DUniform& MeshRenderer::AddSampler2DUniform(
		std::string_view name, Shader::Stage stage, Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(m_Material->GetGraphicsPipeline().GetShader().HasUniform(name, stage),
			"[Material] Sampler2DUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		uint32_t set =
			m_Material->GetGraphicsPipeline().GetShader().GetUniforms(stage)->Get(name)->set;

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

		// Create buffer uniform
		Scope<Sampler2DUniform>& uniform =
			m_Sampler2DUniforms[set].emplace_back(MakeScope<Sampler2DUniform>(
				name, stage, std::move(texture), m_Material->GetGraphicsPipeline()));
		pDescriptor->BindUniform(*uniform);

		return *uniform;
	}

	BufferUniform& MeshRenderer::GetBufferUniform(std::string_view name)
	{
		for (auto& descriptors : m_BufferUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff->GetName() == name)
					return *uBuff;
		RAY_THROW_RUNTIME("[MeshRenderer] Failed to retrieve BufferUniform \"{0}\"", name);
	}

	Sampler2DUniform& MeshRenderer::GetSampler2DUniform(std::string_view name)
	{
		for (auto& descriptors : m_Sampler2DUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff->GetName() == name)
					return *uBuff;
		RAY_THROW_RUNTIME("[MeshRenderer] Failed to retrieve Sampler2DUniform \"{0}\"", name);
	}

	void MeshRenderer::SetMaterial(Ref<Material> material)
	{
		m_Material = std::move(material);
		AddUniforms();
	}

	void MeshRenderer::AddUniforms()
	{
		AddBufferUniform("PerObjectData", Ray::Shader::Stage::Vertex);

		auto addSamplerUniforms = [this](std::string_view uniformName, Shader::Stage stage) {
			if (uniformName == UniformTag::AlbedoMapSampler)
				AddSampler2DUniform(
					UniformTag::AlbedoMapSampler, stage, m_Material->GetAlbedoMap());
			else if (uniformName == UniformTag::DiffuseMapSampler)
				AddSampler2DUniform(
					UniformTag::DiffuseMapSampler, stage, m_Material->GetDiffuseMap());
			else if (uniformName == UniformTag::SpecularMapSampler)
				AddSampler2DUniform(
					UniformTag::SpecularMapSampler, stage, m_Material->GetSpecularMap());
			else if (uniformName == UniformTag::NormalMapSampler)
				AddSampler2DUniform(
					UniformTag::NormalMapSampler, stage, m_Material->GetNormalMap());
			else
				RAY_ASSERT(
					false, "[MeshRenderer] Unknown predefined uniform name \"{0}\"", uniformName);
		};

		for (const auto& [stage, stageData] :
			m_Material->GetGraphicsPipeline().GetShader().GetShaderData())
		{
			for (const auto& uBlock : stageData.uniformBlocks)
			{
				if (uBlock.uniformBlockName == UniformTag::PerObjectData)
					AddBufferUniform(UniformTag::PerObjectData, stage);
				else if (uBlock.uniformBlockName == UniformTag::Shading)
					AddBufferUniform(UniformTag::Shading, stage);
			}

			// Only sampler uniforms can be outside of a block
			for (const auto& uniform : stageData.uniforms)
			{
				addSamplerUniforms(uniform.uniformName, stage);
			}
		}
	}
}  // namespace At0::Ray
