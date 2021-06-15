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
		m_PerObjectDataUniformRef =
			AddBufferUniform(UniformTag::PerObjectData, Ray::ShaderStage::Vertex)["Model"];
	}
}  // namespace At0::Ray
