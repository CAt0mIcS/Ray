#include "Rpch.h"
#include "RUniformAccess.h"

#include "RShader.h"
#include "RPipeline.h"


namespace At0::Ray
{
	UniformAccess::UniformAccess(const Pipeline& pipeline) : m_Shader(pipeline.GetShader()) {}

	UniformAccess::UniformData UniformAccess::Resolve(
		Shader::Stage stageFlag, std::string_view uniformName, std::string_view uniformBlockName)
	{
		// If no uniform block was specified the uniform is in global space
		if (uniformBlockName.empty())
		{
			auto uniforms = m_Shader.GetUniforms(stageFlag);
			auto uniformData = uniforms->Get(uniformName);

			return { uniformData->offset };
		}
		else
		{
			auto uniformBlocks = m_Shader.GetUniformBlocks(stageFlag);
			auto uniformBlockData = uniformBlocks->Get(uniformBlockName);
			auto uniformData = uniformBlockData->uniforms.Get(uniformName);

			return { uniformData->offset };
		}
	}
}  // namespace At0::Ray
