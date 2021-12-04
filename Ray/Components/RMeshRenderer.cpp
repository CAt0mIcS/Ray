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
	MeshRenderer::MeshRenderer(Entity entity, Ref<Material> material)
		: Component(entity), Renderer(std::move(material))
	{
		m_PerObjectDataUniformRef = GetBufferUniform(UniformBlockTag::PerObjectData)["Model"];

		if (m_DirtyListenerIndex == (uint32_t)-1)
			m_DirtyListenerIndex = m_Material->AddOnDirtyListener(
				[this](const std::string& uniformPath, UniformType uType)
				{ UpdateMaterialData(uniformPath, uType); });

		SetMaterialData();
	}

	// MeshRenderer::~MeshRenderer()
	//{
	//	if (m_DirtyListenerIndex != (uint32_t)-1)
	//		m_Material->RemoveOnDirtyListener(m_DirtyListenerIndex);
	//}

	// MeshRenderer::MeshRenderer(MeshRenderer&& other) noexcept
	//	: Component(other.GetEntity()), Renderer(other.m_Material)
	//{
	//	other.m_Material->RemoveOnDirtyListener(other.m_DirtyListenerIndex);
	//	other.m_DirtyListenerIndex = (uint32_t)-1;

	//	m_PerObjectDataUniformRef = std::move(other.m_PerObjectDataUniformRef);
	//	m_DirtyListenerIndex =
	//		m_Material->AddOnDirtyListener([this](const std::string& uniformPath, UniformType uType)
	//			{ UpdateMaterialData(uniformPath, uType); });
	//}

	// MeshRenderer& MeshRenderer::operator=(MeshRenderer&& other) noexcept
	//{
	//	*this = MeshRenderer(std::move(other));
	//	return *this;
	//}

	void MeshRenderer::Render(const CommandBuffer& cmdBuff) const
	{
		// if (!Scene::Get().GetCamera().GetFrustum().SphereCheck(
		//		GetEntity().Get<Transform>().Translation(), 1.0f /*radius*/))
		//	return;

		m_Material->CmdBind(cmdBuff);

		for (const auto& descSet : m_DescriptorSets)
			descSet.CmdBind(cmdBuff);
	}

	void MeshRenderer::Update()
	{
		RAY_MEXPECTS(m_PerObjectDataUniformRef,
			"[MeshRenderer] Mandatory BufferUniform \"{0}\" was not added",
			UniformBlockTag::PerObjectData);

		if (auto& tform = GetEntity().Get<Transform>(); tform.HasChanged())
		{
			if (GetEntity().HasParent())
				(*m_PerObjectDataUniformRef) =
					GetEntity().GetParent().Get<Transform>().AsMatrix() * tform.AsMatrix();
			else
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
				if (!m_Material->HasUniformBlock(uBlockData.name))
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
		}
	}

	void MeshRenderer::UpdateUniform(const std::string& dataPath)
	{
		int pos = dataPath.find('.');
		std::string_view uBlockName = dataPath.substr(0, pos);
		std::string uName = dataPath.substr(pos + 1);
		switch (m_Material->GetType(dataPath))
		{
		case ShaderDataType::Float:
			GetBufferUniform(uBlockName)[uName] = *(float*)m_Material->GetRaw(dataPath);
			break;
		case ShaderDataType::Int:
			GetBufferUniform(uBlockName)[uName] = *(int32_t*)m_Material->GetRaw(dataPath);
			break;
		case ShaderDataType::UInt:
			GetBufferUniform(uBlockName)[uName] = *(uint32_t*)m_Material->GetRaw(dataPath);
			break;
		case ShaderDataType::Vec2:
			GetBufferUniform(uBlockName)[uName] = *(Float2*)m_Material->GetRaw(dataPath);
			break;
		case ShaderDataType::Vec3:
			GetBufferUniform(uBlockName)[uName] = *(Float3*)m_Material->GetRaw(dataPath);
			break;
		case ShaderDataType::Vec4:
			GetBufferUniform(uBlockName)[uName] = *(Float4*)m_Material->GetRaw(dataPath);
			break;
		case ShaderDataType::Mat3:
			GetBufferUniform(uBlockName)[uName] = *(Matrix3*)m_Material->GetRaw(dataPath);
			break;
		case ShaderDataType::Mat4:
			GetBufferUniform(uBlockName)[uName] = *(Matrix*)m_Material->GetRaw(dataPath);
			break;
		default:
			RAY_ASSERT(
				false, "[MeshRenderer] Data type {0} unknown.", (int)m_Material->GetType(dataPath));
		}
	}
}  // namespace At0::Ray
