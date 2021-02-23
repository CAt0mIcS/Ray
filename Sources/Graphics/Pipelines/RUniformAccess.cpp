#include "Rpch.h"
#include "RUniformAccess.h"

#include "RShader.h"
#include "RPipeline.h"

#include "Utils/RString.h"
#include "Utils/RException.h"


namespace At0::Ray
{
	UniformAccess::UniformAccess(const Pipeline& pipeline) : m_Shader(pipeline.GetShader()) {}

	UniformAccess::UniformDataAccess UniformAccess::Resolve(Shader::Stage stageFlag)
	{
		return { m_Shader.GetUniforms(stageFlag), m_Shader.GetUniformBlocks(stageFlag) };
	}

	UniformAccess::UniformData UniformAccess::UniformDataAccess::operator()(
		std::string_view uniformBlockName, std::string_view uniformName)
	{
		if (!m_UniformBlocks)
			RAY_THROW_RUNTIME("[UniformDataAccess] No uniform blocks registered for shader stage.");

		auto uniformBlockData = m_UniformBlocks->Get(uniformBlockName);
		if (!uniformBlockData)
			RAY_THROW_RUNTIME("[UniformAccess] Uniform Block {0} was not found.", uniformBlockName);

		auto uniformData = uniformBlockData->uniforms.Get(uniformName);
		if (!uniformData)
			RAY_THROW_RUNTIME("[UniformAccess] Uniform {0} was not found.", uniformName);

		return { uniformData->offset };
	}

	UniformAccess::UniformData UniformAccess::UniformDataAccess::operator()(
		std::string_view uniformName)
	{
		if (!m_Uniforms)
			RAY_THROW_RUNTIME("[UniformDataAccess] No uniforms registered for shader stage.");

		auto uniformData = m_Uniforms->Get(uniformName);
		if (!uniformData)
			RAY_THROW_RUNTIME("[UniformAccess] Uniform {0} was not found.", uniformName);

		return { uniformData->offset };
	}
}  // namespace At0::Ray
