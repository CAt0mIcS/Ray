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
		: Component(entity), Renderer(std::move(material))
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
				{
					m_PerObjectDataUniformRef =
						AddBufferUniform(UniformTag::PerObjectData, stage)["Model"];
				}
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
