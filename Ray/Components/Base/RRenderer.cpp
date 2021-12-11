#include "Rpch.h"
#include "RRenderer.h"

#include "Shading/RMaterial.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Shader/DataAccess/RDescriptor.h"
#include "Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h"
#include "Graphics/Pipelines/Shader/DataAccess/RSampler2DUniform.h"

#include "Utils/RException.h"
#include "Utils/RAssert.h"


namespace At0::Ray
{
	Renderer& Renderer::operator=(Renderer&& other) noexcept
	{
		m_DescriptorSets = std::move(other.m_DescriptorSets);
		m_BufferUniforms = std::move(other.m_BufferUniforms);
		m_Sampler2DUniforms = std::move(other.m_Sampler2DUniforms);
		m_PushConstants = std::move(other.m_PushConstants);
		m_Material = std::move(other.m_Material);
		return *this;
	}

	Renderer::Renderer(Renderer&& other) noexcept { *this = std::move(other); }

	Renderer::~Renderer() {}

	bool Renderer::HasBufferUniform(std::string_view name) const
	{
		for (auto& [set, uniforms] : m_BufferUniforms)
			for (const BufferUniform& uniform : uniforms)
				if (uniform.GetName() == name)
					return true;
		return false;
	}

	bool Renderer::HasSampler2DUniform(std::string_view name) const
	{
		for (auto& [set, uniforms] : m_Sampler2DUniforms)
			for (const Sampler2DUniform& uniform : uniforms)
				if (uniform.GetName() == name)
					return true;
		return false;
	}

	bool Renderer::HasPushConstant(std::string_view name) const
	{
		return std::find_if(m_PushConstants.begin(), m_PushConstants.end(),
				   [name](const BufferUniform& uniform)
				   { return uniform.GetName() == name; }) != m_PushConstants.end();
	}

	Renderer::Renderer(Ref<Material> material) : m_Material(material) { AddUniforms(); }

	BufferUniform& Renderer::AddBufferUniform(const std::string& name, ShaderStage stage)
	{
		RAY_MEXPECTS(
			m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage).HasUniformBlock(
				name),
			"[Renderer] BufferUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));
		RAY_MEXPECTS(
			!HasBufferUniform(name), "[Renderer] Buffer Uniform \"{0}\" was already added", name);

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

		RAY_MEXPECTS(std::find_if(m_BufferUniforms[set].begin(), m_BufferUniforms[set].end(),
						 [&name](const auto& uniform)
						 { return uniform.GetName() == name; }) == m_BufferUniforms[set].end(),
			"[Renderer] BufferUniform \"{0}\" already added", name);

		// Create buffer uniform
		BufferUniform& uniform = m_BufferUniforms[set].emplace_back(
			name.data(), stage, m_Material->GetGraphicsPipeline());
		pDescriptor->BindUniform(uniform);

		return uniform;
	}

	Sampler2DUniform& Renderer::AddSampler2DUniform(
		const std::string& name, ShaderStage stage, Ref<Texture2D> texture)
	{
		RAY_MEXPECTS(m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage).HasUniform(
						 name, true),
			"[Renderer] Sampler2DUniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));
		RAY_MEXPECTS(!HasSampler2DUniform(name),
			"[Renderer] Sampler2D Uniform \"{0}\" was already added", name);

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

		RAY_MEXPECTS(std::find_if(m_Sampler2DUniforms[set].begin(), m_Sampler2DUniforms[set].end(),
						 [&name](const auto& uniform)
						 { return uniform.GetName() == name; }) == m_Sampler2DUniforms[set].end(),
			"[Renderer] Sampler2DUniform \"{0}\" already added", name);

		// Create buffer uniform
		Sampler2DUniform& uniform = m_Sampler2DUniforms[set].emplace_back(
			name.data(), stage, texture, m_Material->GetGraphicsPipeline());
		if (texture)
			pDescriptor->BindUniform(uniform);

		return uniform;
	}

	BufferUniform& Renderer::AddPushConstant(std::string_view name, ShaderStage stage)
	{
		RAY_MEXPECTS(
			m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage).HasUniformBlock(
				name),
			"[Renderer] Push Constant \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));
		RAY_MEXPECTS(
			!HasPushConstant(name), "[Renderer] Push Constant \"{0}\" was already added", name);
		return m_PushConstants.emplace_back(name, stage, m_Material->GetGraphicsPipeline());
	}

	BufferUniform& Renderer::GetBufferUniform(std::string_view name)
	{
		for (auto& descriptors : m_BufferUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff.GetName() == name)
					return uBuff;
		ThrowRuntime("[Renderer] Failed to retrieve BufferUniform \"{0}\"", name);
		return m_BufferUniforms[0][0];
	}

	Sampler2DUniform& Renderer::GetSampler2DUniform(std::string_view name)
	{
		for (auto& descriptors : m_Sampler2DUniforms)
			for (auto& uBuff : descriptors.second)
				if (uBuff.GetName() == name)
					return uBuff;
		ThrowRuntime("[Renderer] Failed to retrieve Sampler2DUniform \"{0}\"", name);
		return m_Sampler2DUniforms[0][0];
	}

	BufferUniform& Renderer::GetPushConstant(std::string_view name)
	{
		for (auto& pushConstant : m_PushConstants)
			if (pushConstant.GetName() == name)
				return pushConstant;
		ThrowRuntime("[Renderer] Failed to retrieve push constant \"{0}\"", name);
		return m_PushConstants[0];
	}

	DescriptorSet& Renderer::GetDescriptorSet(std::string_view uniformName)
	{
		for (const auto& [set, uniforms] : m_BufferUniforms)
			for (const auto& uniform : uniforms)
				if (uniform.GetName() == uniformName)
					for (auto& descSet : m_DescriptorSets)
						if (descSet.GetSetNumber() == set)
							return descSet;

		for (const auto& [set, uniforms] : m_Sampler2DUniforms)
			for (const auto& uniform : uniforms)
				if (uniform.GetName() == uniformName)
					for (auto& descSet : m_DescriptorSets)
						if (descSet.GetSetNumber() == set)
							return descSet;
		ThrowRuntime("[Renderer] Failed to retrieve Descriptor Set of uniform with name \"{0}\"",
			uniformName);
		return m_DescriptorSets[0];
	}

	void Renderer::SetSamplerTexture(std::string_view name, Ref<Texture2D> texture)
	{
		for (auto& [set, uniforms] : m_Sampler2DUniforms)
			for (auto& uniform : uniforms)
				if (uniform.GetName() == name)
					for (auto& descSet : m_DescriptorSets)
						if (descSet.GetSetNumber() == set)
						{
							uniform.SetTexture(std::move(texture), descSet);
							return;
						}

		ThrowRuntime("[Renderer] Failed to find Sampler2DUniform with name \"{0}\"", name);
	}

	void Renderer::AddUniforms()
	{
		for (ShaderStage stage :
			m_Material->GetGraphicsPipeline().GetShader().GetLiveShaderStages())
		{
			const ShaderReflection& reflection =
				m_Material->GetGraphicsPipeline().GetShader().GetReflection(stage);

			for (const auto& uniform : reflection.GetUniforms())
			{
				switch (uniform.type)
				{
				case UniformType::CombinedImageSampler:
					AddSampler2DUniform(uniform.name, stage, nullptr);
					break;
				default: RAY_ASSERT(false, "[Renderer] Invalid uniform type");
				}
			}

			for (const auto& uniformBlock : reflection.GetUniformBlocks())
			{
				switch (uniformBlock.type)
				{
				case UniformType::UniformBuffer: AddBufferUniform(uniformBlock.name, stage); break;
				case UniformType::Push: AddPushConstant(uniformBlock.name, stage); break;
				case UniformType::CombinedImageSampler:
					AddSampler2DUniform(uniformBlock.name, stage, nullptr);
					break;
				}
			}
		}
	}
}  // namespace At0::Ray
