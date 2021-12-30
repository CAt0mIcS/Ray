#include "RSampler2DUniform.h"

#include "../../RPipeline.h"
#include "../../Shader/RShader.h"
#include "RDescriptor.h"


namespace At0::Ray
{
	Sampler2DUniform::Sampler2DUniform(
		std::string_view name, ShaderStage stage, const Pipeline& pipeline)
		: m_Name(name)
	{
		RAY_MEXPECTS(pipeline.GetShader().GetReflection(stage).HasUniform(name, true),
			"[BufferUniform] Uniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		const auto& uniform = pipeline.GetShader().GetReflection(stage).GetUniform(name);
		m_Binding = uniform.binding;
	}

	Sampler2DUniform::Sampler2DUniform(std::string_view name, uint32_t binding)
		: m_Name(name), m_Binding(binding)
	{
	}

	void Sampler2DUniform::SetTexture(Ref<Texture> texture, DescriptorSet& descSet)
	{
		descSet.BindUniform(*this, std::move(texture));
	}
}  // namespace At0::Ray
