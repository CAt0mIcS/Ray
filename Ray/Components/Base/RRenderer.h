#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../../Graphics/Pipelines/Shader/RShaderTypes.h"

#include <string>
#include <unordered_map>
#include <vector>


namespace At0::Ray
{
	class DescriptorSet;
	class BufferUniform;
	class Sampler2DUniform;
	class Texture;
	class GraphicsPipeline;
	class Material;
	enum class ShaderStage;

	class RAY_EXPORT Renderer : NonCopyable
	{
	public:
		Renderer& operator=(Renderer&& other) noexcept;
		Renderer(Renderer&& other) noexcept;

		virtual ~Renderer();

		BufferUniform& AddBufferUniform(const std::string& name, ShaderStage stage);
		Sampler2DUniform& AddSampler2DUniform(
			const std::string& name, ShaderStage stage, Ref<Texture> texture);
		BufferUniform& AddPushConstant(std::string_view name, ShaderStage stage);

		BufferUniform& GetBufferUniform(std::string_view name);
		Sampler2DUniform& GetSampler2DUniform(std::string_view name);
		BufferUniform& GetPushConstant(std::string_view name);

		DescriptorSet& GetDescriptorSet(uint32_t set);

		void SetMaterial(Ref<Material> material) { m_Material = std::move(material); }
		void SetSamplerTexture(std::string_view name, Ref<Texture> texture,
			VkImageLayout imageLayout = VK_IMAGE_LAYOUT_MAX_ENUM);

		Material& GetMaterial() { return *m_Material; }
		const Material& GetMaterial() const { return *m_Material; }
		Ref<Material> GetSharedMaterial() const { return m_Material; }

		bool HasBufferUniform(std::string_view name) const;
		bool HasSampler2DUniform(std::string_view name) const;
		bool HasPushConstant(std::string_view name) const;

	protected:
		Renderer(Ref<Material> material);

	private:
		void AddUniforms();

	protected:
		std::vector<DescriptorSet> m_DescriptorSets;
		std::unordered_map<uint32_t, std::vector<BufferUniform>> m_BufferUniforms;
		std::unordered_map<uint32_t, std::vector<Sampler2DUniform>> m_Sampler2DUniforms;

		std::vector<BufferUniform> m_PushConstants;

		Ref<Material> m_Material;
	};
}  // namespace At0::Ray
