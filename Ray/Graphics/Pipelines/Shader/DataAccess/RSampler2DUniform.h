#pragma once

#include "../../../../RBase.h"


namespace At0::Ray
{
	class Pipeline;
	class DescriptorSet;
	class Texture;
	enum class ShaderStage;

	class RAY_EXPORT Sampler2DUniform
	{
	public:
		Sampler2DUniform(std::string_view name, ShaderStage stage, const Pipeline& pipeline);
		Sampler2DUniform(std::string_view name, uint32_t binding);

		std::string_view GetName() const { return m_Name; }
		uint32_t GetBinding() const { return m_Binding; }

		/**
		 * Sets the texture and rebinds descriptor set to uniform
		 */
		void SetTexture(Ref<Texture> texture, DescriptorSet& descSet);

	private:
		std::string m_Name;

		// The binding specified in the shader
		uint32_t m_Binding;
	};
}  // namespace At0::Ray
