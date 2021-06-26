#pragma once

#include "Ray/RBase.h"


namespace At0::Ray
{
	class Pipeline;
	class Texture2D;
	class DescriptorSet;
	enum class ShaderStage;

	class RAY_EXPORT Sampler2DUniform
	{
	public:
		Sampler2DUniform(std::string_view name, ShaderStage stage, Ref<Texture2D> texture,
			const Pipeline& pipeline);
		Sampler2DUniform(std::string_view name, Ref<Texture2D> texture, uint32_t binding);

		std::string_view GetName() const { return m_Name; }
		uint32_t GetBinding() const { return m_Binding; }
		Ref<Texture2D> GetTexture() const { return m_Texture; }
		void SetTexture(Ref<Texture2D> texture, DescriptorSet& descSet);

	private:
		std::string m_Name;

		// The binding specified in the shader
		uint32_t m_Binding;
		// The texture associated with the sampler uniform
		Ref<Texture2D> m_Texture;
	};
}  // namespace At0::Ray
