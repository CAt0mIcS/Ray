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

	UniformAccess::UniformData UniformAccess::UniformDataAccess::operator[](
		std::string_view uniformPath)
	{
		std::vector<std::string> uniformBlockAndName = String::Split(uniformPath, '.');
		if (uniformBlockAndName.size() > 2)
			RAY_THROW_RUNTIME("[UniformDataAccess] Uniform path {0} is invalid.", uniformPath);

		// If the string doesn't contain a '.', assume that the uniform is global
		if (uniformBlockAndName.size() == 1)
		{
			if (!m_Uniforms)
				RAY_THROW_RUNTIME("[UniformDataAccess] No uniforms registered for shader stage.");

			auto uniformData = m_Uniforms->Get(uniformBlockAndName[0]);
			if (!uniformData)
				RAY_THROW_RUNTIME(
					"[UniformAccess] Uniform {0} was not found.", uniformBlockAndName[0]);

			return { uniformData->offset };
		}
		else
		{
			if (!m_UniformBlocks)
				RAY_THROW_RUNTIME(
					"[UniformDataAccess] No uniform blocks registered for shader stage.");

			auto uniformBlockData = m_UniformBlocks->Get(uniformBlockAndName[0]);
			if (!uniformBlockData)
				RAY_THROW_RUNTIME(
					"[UniformAccess] Uniform Block {0} was not found.", uniformBlockAndName[0]);

			auto uniformData = uniformBlockData->uniforms.Get(uniformBlockAndName[1]);
			if (!uniformData)
				RAY_THROW_RUNTIME(
					"[UniformAccess] Uniform {0} was not found.", uniformBlockAndName[1]);

			return { uniformData->offset };
		}
	}
}  // namespace At0::Ray
