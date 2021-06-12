#include "Rpch.h"
#include "RSampler2DUniform.h"

#include "../RPipeline.h"
#include "../Shader/RShader.h"
#include "Graphics/Images/RTexture2D.h"

#include "Utils/RAssert.h"
#include "Utils/RString.h"


namespace At0::Ray
{
	Sampler2DUniform::Sampler2DUniform(
		std::string_view name, ShaderStage stage, Ref<Texture2D> texture, const Pipeline& pipeline)
		: m_Texture(std::move(texture)), m_Name(name)
	{
		RAY_MEXPECTS(pipeline.GetShader().GetReflection(stage).HasUniform(name, true),
			"[BufferUniform] Uniform \"{0}\" was not found in shader stage \"{1}\"", name,
			String::Construct(stage));

		const auto& uniform = pipeline.GetShader().GetReflection(stage).GetUniform(name);
		m_Binding = uniform.binding;
	}

	Sampler2DUniform::Sampler2DUniform(
		std::string_view name, Ref<Texture2D> texture, uint32_t binding)
		: m_Name(name), m_Texture(std::move(texture)), m_Binding(binding)
	{
	}
}  // namespace At0::Ray
