#include "Rpch.h"
#include "RMeshRenderer.h"

#include "RTransform.h"
#include "RMesh.h"

#include "Scene/RScene.h"
#include "Shading/RMaterial.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Shader/RShader.h"
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
		// if (!Scene::Get().GetCamera().GetFrustum().SphereCheck(
		//		GetEntity().Get<Transform>().Translation(), 1.0f /*radius*/))
		//	return;

		m_Material->CmdBind(cmdBuff);

		for (const auto& descSet : m_DescriptorSets)
			descSet.CmdBind(cmdBuff);

		// RAY_TODO
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

	BufferUniform& MeshRenderer::AddBufferUniform(const std::string& name, ShaderStage stage)
	{
		RAY_MEXPECTS(
			m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage).HasUniformBlock(
				name),
			"[Material] BufferUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

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

		// Create buffer uniform
		Scope<BufferUniform>& uniform = m_BufferUniforms[set].emplace_back(
			MakeScope<BufferUniform>(name, stage, m_Material->GetGraphicsPipeline()));
		pDescriptor->BindUniform(*uniform);

		if (name == UniformTag::PerObjectData)
			m_PerObjectDataUniformRef = (*uniform)["Model"];

		return *uniform;
	}

	Sampler2DUniform& MeshRenderer::AddSampler2DUniform(
		const std::string& name, ShaderStage stage, Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage).HasUniform(
						 name, true),
			"[Material] Sampler2DUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

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
		ThrowRuntime("[MeshRenderer] Failed to retrieve BufferUniform \"{0}\"", name);
		return *m_BufferUniforms[0][0];
	}

	Sampler2DUniform& MeshRenderer::GetSampler2DUniform(std::string_view name)
	{
		for (auto& descriptors : m_Sampler2DUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff->GetName() == name)
					return *uBuff;
		ThrowRuntime("[MeshRenderer] Failed to retrieve Sampler2DUniform \"{0}\"", name);
		return *m_Sampler2DUniforms[0][0];
	}

	void MeshRenderer::SetMaterial(Ref<Material> material)
	{
		m_Material = std::move(material);
		AddUniforms();
	}

	void MeshRenderer::AddUniforms()
	{
		AddBufferUniform("PerObjectData", Ray::ShaderStage::Vertex);

		auto addSamplerUniforms = [this](std::string_view uniformName, ShaderStage stage) {
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

		for (const auto& [stage, reflection] :
			m_Material->GetGraphicsPipeline().GetShader().GetReflections())
		{
			for (const auto& uBlock : reflection.GetUniformBlocks())
			{
				if (uBlock.name == UniformTag::PerObjectData)
					AddBufferUniform(UniformTag::PerObjectData, stage);
				else if (uBlock.name == UniformTag::Shading)
					AddBufferUniform(UniformTag::Shading, stage);
			}

			// Only sampler uniforms can be outside of a block
			for (const auto& uniform : reflection.GetUniforms())
			{
				addSamplerUniforms(uniform.name, stage);
			}
		}
	}
}  // namespace At0::Ray
