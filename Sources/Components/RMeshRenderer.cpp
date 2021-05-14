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
			"[Material] BufferUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		uint32_t set =
			m_Material->GetGraphicsPipeline().GetShader().GetUniformBlocks(stage)->Get(name)->set;

		// Create descriptor set if the one for this set does not exist yet
		if (!m_DescriptorSets[set])
			m_DescriptorSets[set] = MakeScope<DescriptorSet>(
				m_Material->GetGraphicsPipeline().GetDescriptorPool(),
				m_Material->GetGraphicsPipeline().GetDescriptorSetLayout(set),
				Pipeline::BindPoint::Graphics, m_Material->GetGraphicsPipeline().GetLayout(), set);

		// Create buffer uniform
		Scope<BufferUniform>& uniform = m_BufferUniforms[set].emplace_back(
			MakeScope<BufferUniform>(name, stage, m_Material->GetGraphicsPipeline()));
		m_DescriptorSets[set]->BindUniform(*uniform);

		return *uniform;
	}

	Sampler2DUniform& MeshRenderer::AddSampler2DUniform(
		std::string_view name, Shader::Stage stage, Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(m_Material->GetGraphicsPipeline().GetShader().HasUniform(name, stage),
			"[Material] Sampler2DUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		uint32_t set =
			m_Material->GetGraphicsPipeline().GetShader().GetUniformBlocks(stage)->Get(name)->set;

		// Create descriptor set if the one for this set does not exist yet
		if (!m_DescriptorSets[set])
			m_DescriptorSets[set] = MakeScope<DescriptorSet>(
				m_Material->GetGraphicsPipeline().GetDescriptorPool(),
				m_Material->GetGraphicsPipeline().GetDescriptorSetLayout(set),
				Pipeline::BindPoint::Graphics, m_Material->GetGraphicsPipeline().GetLayout(), set);

		// Create buffer uniform
		Scope<Sampler2DUniform>& uniform =
			m_Sampler2DUniforms[set].emplace_back(MakeScope<Sampler2DUniform>(
				name, stage, std::move(texture), m_Material->GetGraphicsPipeline()));
		m_DescriptorSets[set]->BindUniform(*uniform);

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
}  // namespace At0::Ray
