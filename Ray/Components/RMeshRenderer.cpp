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
		UpdateMaterialData();
	}

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

	void MeshRenderer::UpdateMaterialData()
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
					{
						std::string dataPath = uBlockData.name + '.' + uniform.name;
						switch (m_Material->GetType(dataPath))
						{
						case ShaderDataType::Float:
							GetBufferUniform(uBlockData.name)[uniform.name] =
								*(float*)m_Material->Get({ uBlockData.name + '.' + uniform.name });
							break;
						case ShaderDataType::Int:
							GetBufferUniform(uBlockData.name)[uniform.name] = *(
								int32_t*)m_Material->Get({ uBlockData.name + '.' + uniform.name });
							break;
						case ShaderDataType::UInt:
							GetBufferUniform(uBlockData.name)[uniform.name] = *(
								uint32_t*)m_Material->Get({ uBlockData.name + '.' + uniform.name });
							break;
						case ShaderDataType::Vec2:
							GetBufferUniform(uBlockData.name)[uniform.name] =
								*(Float2*)m_Material->Get({ uBlockData.name + '.' + uniform.name });
							break;
						case ShaderDataType::Vec3:
							GetBufferUniform(uBlockData.name)[uniform.name] =
								*(Float3*)m_Material->Get({ uBlockData.name + '.' + uniform.name });
							break;
						case ShaderDataType::Vec4:
							GetBufferUniform(uBlockData.name)[uniform.name] =
								*(Float4*)m_Material->Get({ uBlockData.name + '.' + uniform.name });
							break;
						case ShaderDataType::Mat3:
							GetBufferUniform(uBlockData.name)[uniform.name] = *(
								Matrix3*)m_Material->Get({ uBlockData.name + '.' + uniform.name });
							break;
						case ShaderDataType::Mat4:
							GetBufferUniform(uBlockData.name)[uniform.name] =
								*(Matrix*)m_Material->Get({ uBlockData.name + '.' + uniform.name });
							break;
						default:
							RAY_ASSERT(false, "[MeshRenderer] Data type {0} unknown.",
								(int)m_Material->GetType(dataPath));
						}
					}
				}
			}

			for (const auto& uniform : reflection.GetUniforms())
				if (m_Material->HasUniform(uniform.name))
					SetSamplerTexture(uniform.name, m_Material->GetTexture(uniform.name));
		}
	}
}  // namespace At0::Ray
