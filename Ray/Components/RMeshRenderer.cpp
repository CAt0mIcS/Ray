#include "RMeshRenderer.h"

#include "RTransform.h"
#include "RMesh.h"

#include "Scene/RScene.h"
#include "Scene/RCamera.h"
#include "Shading/RMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Shader/DataAccess/RDescriptor.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h"


namespace At0::Ray
{
	MeshRenderer::MeshRenderer(Entity entity, Ref<Material> material)
		: Component(entity),
		  Renderer(std::move(material)), EventListener<MaterialBecameDirtyEvent>(*m_Material),
		  EventListener<CameraChangedEvent>(Scene::Get().GetCamera()),
		  m_PerObjectDataUniformRef(GetBufferUniform(UniformBlockTag::PerObjectData)["Model"])
	{
		m_PerSceneUniform = &GetBufferUniform(UniformBlockTag::PerSceneData);

		SetMaterialData();

		// Set Camera data
		CameraChangedEvent e{};
		OnEvent(e);
	}

	void MeshRenderer::Render(const CommandBuffer& cmdBuff) const
	{
		// if (!Scene::Get().GetCamera().GetFrustum().SphereCheck(
		//		GetEntity().Get<Transform>().Translation(), 1.0f /*radius*/))
		//	return;
		RAY_DEBUG_FLAG(m_Name = GetEntity().Get<Mesh>().GetName());

		m_Material->CmdBind(cmdBuff);

		for (const auto& descSet : m_DescriptorSets)
			descSet.CmdBind(cmdBuff);

		for (const auto& pushConstant : m_PushConstants)
			pushConstant.CmdBindAsPushConstant(
				cmdBuff, m_Material->GetGraphicsPipeline().GetLayout());
	}

	void MeshRenderer::Update()
	{
		if (auto& tform = GetEntity().Get<Transform>(); tform.HasChanged())
		{
			// RAY_TODO: Deleting entity while checking here! Threading error
			if (GetEntity().HasParent())
				m_PerObjectDataUniformRef =
					GetEntity().GetParent().Get<Transform>().AsMatrix() * tform.AsMatrix();
			else
				m_PerObjectDataUniformRef = tform.AsMatrix();
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
						UpdateUniform(uBlockData.name + '.' + uniform.name,
							uBlockData.type == UniformType::Push);
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
		case UniformType::UniformBuffer: UpdateUniform(uniformPath, false); break;
		case UniformType::CombinedImageSampler:
			SetSamplerTexture(uniformPath, m_Material->GetTexture(uniformPath));
			break;
		case UniformType::Push: UpdateUniform(uniformPath, true); break;
		}
	}

	void MeshRenderer::UpdateUniform(const std::string& dataPath, bool isPushConstant)
	{
		int pos = dataPath.find('.');
		std::string uBlockName = dataPath.substr(0, pos);
		std::string uName = dataPath.substr(pos + 1);

		auto getUniform = [&]()
		{
			if (isPushConstant)
				return GetPushConstant(uBlockName)[uName];
			return GetBufferUniform(uBlockName)[uName];
		};

		switch (m_Material->GetType(dataPath))
		{
		case ShaderDataType::Float: getUniform() = m_Material->Get<float>(dataPath); break;
		case ShaderDataType::Vec2: getUniform() = m_Material->Get<Float2>(dataPath); break;
		case ShaderDataType::Vec3: getUniform() = m_Material->Get<Float3>(dataPath); break;
		case ShaderDataType::Vec4: getUniform() = m_Material->Get<Float4>(dataPath); break;

		case ShaderDataType::Int: getUniform() = m_Material->Get<int32_t>(dataPath); break;
		case ShaderDataType::IVec2: getUniform() = m_Material->Get<Int2>(dataPath); break;
		case ShaderDataType::IVec3: getUniform() = m_Material->Get<Int3>(dataPath); break;
		case ShaderDataType::IVec4: getUniform() = m_Material->Get<Int4>(dataPath); break;

		case ShaderDataType::UInt: getUniform() = m_Material->Get<uint32_t>(dataPath); break;
		case ShaderDataType::UVec2: getUniform() = m_Material->Get<UInt2>(dataPath); break;
		case ShaderDataType::UVec3: getUniform() = m_Material->Get<UInt3>(dataPath); break;
		case ShaderDataType::UVec4: getUniform() = m_Material->Get<UInt4>(dataPath); break;

		case ShaderDataType::Bool: getUniform() = m_Material->Get<VkBool32>(dataPath); break;
		case ShaderDataType::BVec2: getUniform() = m_Material->Get<Bool2>(dataPath); break;
		case ShaderDataType::BVec3: getUniform() = m_Material->Get<Bool3>(dataPath); break;
		case ShaderDataType::BVec4: getUniform() = m_Material->Get<Bool4>(dataPath); break;

		case ShaderDataType::Double: getUniform() = m_Material->Get<double>(dataPath); break;
		case ShaderDataType::DVec2: getUniform() = m_Material->Get<Double2>(dataPath); break;
		case ShaderDataType::DVec3: getUniform() = m_Material->Get<Double3>(dataPath); break;
		case ShaderDataType::DVec4: getUniform() = m_Material->Get<Double4>(dataPath); break;

		case ShaderDataType::Mat2: getUniform() = m_Material->Get<Matrix2>(dataPath); break;
		case ShaderDataType::Mat2x3: getUniform() = m_Material->Get<Matrix2x3>(dataPath); break;
		case ShaderDataType::Mat2x4: getUniform() = m_Material->Get<Matrix2x4>(dataPath); break;

		case ShaderDataType::Mat3: getUniform() = m_Material->Get<Matrix3>(dataPath); break;
		case ShaderDataType::Mat3x2: getUniform() = m_Material->Get<Matrix3x2>(dataPath); break;
		case ShaderDataType::Mat3x4: getUniform() = m_Material->Get<Matrix3x4>(dataPath); break;

		case ShaderDataType::Mat4: getUniform() = m_Material->Get<Matrix4>(dataPath); break;
		case ShaderDataType::Mat4x2: getUniform() = m_Material->Get<Matrix4x2>(dataPath); break;
		case ShaderDataType::Mat4x3: getUniform() = m_Material->Get<Matrix4x3>(dataPath); break;

		case ShaderDataType::DMat2: getUniform() = m_Material->Get<DMatrix2>(dataPath); break;
		case ShaderDataType::DMat2x3: getUniform() = m_Material->Get<DMatrix2x3>(dataPath); break;
		case ShaderDataType::DMat2x4: getUniform() = m_Material->Get<DMatrix2x4>(dataPath); break;

		case ShaderDataType::DMat3: getUniform() = m_Material->Get<DMatrix3>(dataPath); break;
		case ShaderDataType::DMat3x2: getUniform() = m_Material->Get<DMatrix3x2>(dataPath); break;
		case ShaderDataType::DMat3x4: getUniform() = m_Material->Get<DMatrix3x4>(dataPath); break;

		case ShaderDataType::DMat4: getUniform() = m_Material->Get<DMatrix4>(dataPath); break;
		case ShaderDataType::DMat4x2: getUniform() = m_Material->Get<DMatrix4x2>(dataPath); break;
		case ShaderDataType::DMat4x3: getUniform() = m_Material->Get<DMatrix4x3>(dataPath); break;
		default:
			RAY_ASSERT(
				false, "[MeshRenderer] Data type {0} unknown.", (int)m_Material->GetType(dataPath));
		}
	}

	void MeshRenderer::OnEvent(MaterialBecameDirtyEvent& e)
	{
		UpdateMaterialData(e.dataPath, e.uType);
	}

	void MeshRenderer::OnEvent(CameraChangedEvent& e)
	{
		(*m_PerSceneUniform)["View"] = Scene::Get().GetCamera().ShaderData.View;
		(*m_PerSceneUniform)["Proj"] = Scene::Get().GetCamera().ShaderData.Projection;
		if (m_PerSceneUniform->Has("ViewPos"))
			(*m_PerSceneUniform)["ViewPos"] = Scene::Get().GetCamera().ShaderData.ViewPos;
	}
}  // namespace At0::Ray
