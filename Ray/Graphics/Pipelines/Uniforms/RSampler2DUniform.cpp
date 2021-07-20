#include "Rpch.h"
#include "RSampler2DUniform.h"

#include "../RPipeline.h"
#include "../Shader/RShader.h"
#include "Graphics/Images/RTexture2D.h"
#include "RDescriptor.h"

#include "Ray/Utils/RAssert.h"
#include "Ray/Utils/RString.h"


namespace At0::Ray
{
	Sampler2DUniform::Sampler2DUniform(
		std::string_view name, ShaderStage stage, Ref<Texture2D> texture, const Pipeline& pipeline)
		: m_Texture(std::move(texture)), m_Name(name)
	{
		RAY_MEXPECTS(pipeline.GetShader().GetReflection(stage).HasUniform(name, true),
			"[BufferUniform] Uniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct((RrShaderStageFlags)stage));

		const auto& uniform = pipeline.GetShader().GetReflection(stage).GetUniform(name);
		m_Binding = uniform.binding;
	}

	Sampler2DUniform::Sampler2DUniform(
		std::string_view name, Ref<Texture2D> texture, uint32_t binding)
		: m_Name(name), m_Texture(std::move(texture)), m_Binding(binding)
	{
	}

	void Sampler2DUniform::SetTexture(Ref<Texture2D> texture, DescriptorSet& descSet)
	{
		m_Texture = std::move(texture);
		descSet.BindUniform(*this);
	}
}  // namespace At0::Ray
