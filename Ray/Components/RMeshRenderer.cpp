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

// RAY_TODO: Remove
#include "Graphics/Commands/RCommandBuffer.h"


namespace At0::Ray
{
	MeshRenderer::MeshRenderer(Entity entity, Ref<Material> material)
		: Component(entity),
		  Renderer(std::move(material)), EventListener<MaterialBecameDirtyEvent>(*m_Material),
		  EventListener<CameraChangedEvent>(Scene::Get().GetCamera())
	{
		m_PerObjectDataUniformRef = GetBufferUniform(UniformBlockTag::PerObjectData)["Model"];
		m_PerSceneUniform = &GetBufferUniform(UniformBlockTag::PerSceneData);

		SetMaterialData();
	}

	void MeshRenderer::Render(const CommandBuffer& cmdBuff) const
	{
		// if (!Scene::Get().GetCamera().GetFrustum().SphereCheck(
		//		GetEntity().Get<Transform>().Translation(), 1.0f /*radius*/))
		//	return;

		if (m_IsCameraDirty)
		{
			m_IsCameraDirty = false;
			(*m_PerSceneUniform)["View"] = Scene::Get().GetCamera().ShaderData.View;
			(*m_PerSceneUniform)["Proj"] = Scene::Get().GetCamera().ShaderData.Projection;
			if (m_PerSceneUniform->Has("ViewPos"))
				(*m_PerSceneUniform)["ViewPos"] = Scene::Get().GetCamera().ShaderData.ViewPos;
		}

		m_Material->CmdBind(cmdBuff);

		for (const auto& descSet : m_DescriptorSets)
			descSet.CmdBind(cmdBuff);

		// struct PushConstants
		//{
		//	VkBool32 useColor;
		//	VkBool32 useTexture;
		//};

		// PushConstants pushConstants{ m_Material->Get<VkBool32>("Constants.useColor"),
		//	m_Material->Get<VkBool32>("Constants.useTexture") };

		// vkCmdPushConstants(cmdBuff, m_Material->GetGraphicsPipeline().GetLayout(),
		//	VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstants), &pushConstants);
	}

	void MeshRenderer::Update()
	{
		RAY_MEXPECTS(m_PerObjectDataUniformRef,
			"[MeshRenderer] Mandatory BufferUniform \"{0}\" was not added",
			UniformBlockTag::PerObjectData);

		if (auto& tform = GetEntity().Get<Transform>(); tform.HasChanged())
		{
			// RAY_TODO: Deleting entity while checking here! Threading error
			// if (GetEntity().HasParent())
			//	(*m_PerObjectDataUniformRef) =
			//		GetEntity().GetParent().Get<Transform>().AsMatrix() * tform.AsMatrix();
			// else
			(*m_PerObjectDataUniformRef) = tform.AsMatrix();
		}
	}

	void MeshRenderer::SetMaterialData()
	{
		for (const auto& [stage, reflection] :
			m_Material->GetGraphicsPipeline().GetShader().GetReflections())
		{
			for (const auto& uBlockData : reflection.GetUniformBlocks())
			{
				if (uBlockData.type == UniformType::Push ||
					!m_Material->HasUniformBlock(uBlockData.name))
					continue;
				for (const auto& uniform : uBlockData.uniforms)
				{
					if (m_Material->HasUniform(uniform.name))
						UpdateUniform(uBlockData.name + '.' + uniform.name);
				}
			}

			for (const auto& uniform : reflection.GetUniforms())
				if (m_Material->HasUniform(uniform.name))
					SetSamplerTexture(uniform.name, m_Material->GetTexture(uniform.name));
		}
	}

	void MeshRenderer::UpdateMaterialData(const std::string& uniformPath, UniformType uType)
	{
		switch (uType)
		{
		case UniformType::UniformBuffer: UpdateUniform(uniformPath); break;
		case UniformType::CombinedImageSampler:
			SetSamplerTexture(uniformPath, m_Material->GetTexture(uniformPath));
			break;
		case UniformType::Push: RAY_ASSERT(false, "[MeshRenderer] TODO");
		}
	}

	void MeshRenderer::UpdateUniform(const std::string& dataPath)
	{
		int pos = dataPath.find('.');
		std::string uBlockName = dataPath.substr(0, pos);
		std::string uName = dataPath.substr(pos + 1);
		switch (m_Material->GetType(dataPath))
		{
		case ShaderDataType::Float:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<float>(dataPath);
			break;
		case ShaderDataType::Int:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<int32_t>(dataPath);
			break;
		case ShaderDataType::UInt:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<uint32_t>(dataPath);
			break;
		case ShaderDataType::Bool:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<VkBool32>(dataPath);
			break;
		case ShaderDataType::Vec2:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<Float2>(dataPath);
			break;
		case ShaderDataType::Vec3:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<Float3>(dataPath);
			break;
		case ShaderDataType::Vec4:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<Float4>(dataPath);
			break;
		case ShaderDataType::Mat3:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<Matrix3>(dataPath);
			break;
		case ShaderDataType::Mat4:
			GetBufferUniform(uBlockName)[uName] = m_Material->Get<Matrix>(dataPath);
			break;
		default:
			RAY_ASSERT(
				false, "[MeshRenderer] Data type {0} unknown.", (int)m_Material->GetType(dataPath));
		}
	}

	void MeshRenderer::OnEvent(MaterialBecameDirtyEvent& e)
	{
		UpdateMaterialData(e.dataPath, e.uType);
	}
}  // namespace At0::Ray
