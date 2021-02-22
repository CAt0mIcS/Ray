#include "Rpch.h"
#include "RUniformAccess.h"

#include "RShader.h"
#include "RPipeline.h"

#include "Utils/RString.h"
#include "Utils/RAssert.h"


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
			RAY_MEXPECTS(uniforms, "[UniformAccess] No uniforms registered for shader stage {0}.",
				String::Construct(stageFlag));

			auto uniformData = uniforms->Get(uniformName);
			RAY_MEXPECTS(uniformData, "[UniformAccess] Uniform {0} was not found.", uniformName);

			return { uniformData->offset };
		}
		else
		{
			auto uniformBlocks = m_Shader.GetUniformBlocks(stageFlag);
			RAY_MEXPECTS(uniformBlocks,
				"[UniformAccess] No uniform blocks registered for shader stage {0}.",
				String::Construct(stageFlag));

			auto uniformBlockData = uniformBlocks->Get(uniformBlockName);
			RAY_MEXPECTS(uniformBlockData, "[UniformAccess] Uniform Block {0} was not found.",
				uniformBlockName);

			auto uniformData = uniformBlockData->uniforms.Get(uniformName);
			RAY_MEXPECTS(
				uniformBlockData, "[UniformAccess] Uniform {0} was not found.", uniformName);

			return { uniformData->offset };
		}
	}
}  // namespace At0::Ray
