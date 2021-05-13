#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include "../Utils/RNonCopyable.h"
#include "../Graphics/Core/RBindable.h"
#include "../Graphics/Pipelines/RShader.h"
#include "../Graphics/Pipelines/RGraphicsPipeline.h"
#include "../Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "../Graphics/Pipelines/Uniforms/RSampler2DUniform.h"

#include <vector>
#include <unordered_map>


namespace At0::Ray
{
	class DescriptorSet;
	class Texture2D;

	class RAY_EXPORT Material : public Bindable, NonCopyable
	{
	public:
		struct Layout : public GraphicsPipeline::Layout
		{
			Ref<Texture2D> diffuseMap;
			Ref<Texture2D> specularMap;
			Ref<Texture2D> normalMap;
		};

	public:
		Material(const Material::Layout& layout);
		~Material();

		void CmdBind(const CommandBuffer& cmdBuff) const;

		Ref<BufferUniform> AddBufferUniform(std::string_view name, Shader::Stage stage);
		Ref<Sampler2DUniform> AddSampler2DUniform(
			std::string_view name, Shader::Stage stage, Ref<Texture2D> texture);

		Ref<BufferUniform> GetBufferUniform(std::string_view name) const;
		Ref<Sampler2DUniform> GetSampler2DUniform(std::string_view name) const;

		Ref<GraphicsPipeline> GetGraphicsPipeline() const { return m_GraphicsPipeline; }

		Material& operator=(Material&& other) noexcept;
		Material(Material&& other) noexcept;

	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;

		Ref<Texture2D> m_DiffuseMap;
		Ref<Texture2D> m_SpecularMap;
		Ref<Texture2D> m_NormalMap;

		std::unordered_map<uint32_t, Ref<DescriptorSet>> m_DescriptorSets;
		std::unordered_map<uint32_t, std::vector<Ref<BufferUniform>>> m_BufferUniforms;
		std::unordered_map<uint32_t, std::vector<Ref<Sampler2DUniform>>> m_Sampler2DUniforms;
	};
}  // namespace At0::Ray
