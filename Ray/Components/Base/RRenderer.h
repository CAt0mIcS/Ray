#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"


namespace At0::Ray
{
	class DescriptorSet;
	class BufferUniform;
	class Sampler2DUniform;
	class Texture2D;
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
			const std::string& name, ShaderStage stage, Ref<Texture2D> texture);

		BufferUniform& GetBufferUniform(std::string_view name);
		Sampler2DUniform& GetSampler2DUniform(std::string_view name);

		DescriptorSet& GetDescriptorSet(std::string_view uniformName);

		void SetMaterial(Ref<Material> material) { m_Material = std::move(material); }

		Material& GetMaterial() { return *m_Material; }
		const Material& GetMaterial() const { return *m_Material; }
		Ref<Material> GetSharedMaterial() const { return m_Material; }

	protected:
		Renderer(Ref<Material> material);

	protected:
		std::vector<DescriptorSet> m_DescriptorSets;
		std::unordered_map<uint32_t, std::vector<BufferUniform>> m_BufferUniforms;
		std::unordered_map<uint32_t, std::vector<Sampler2DUniform>> m_Sampler2DUniforms;

		Ref<Material> m_Material;
	};
}  // namespace At0::Ray
