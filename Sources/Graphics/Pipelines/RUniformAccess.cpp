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
		switch (stageFlag)
		{
		case Shader::Stage::Vertex:
		{
			// If no uniform block was specified the uniform is in global space
			if (uniformBlockName.empty())
			{
				auto uniforms = m_Shader.GetUniforms(Shader::Stage::Vertex);
				auto uniformData = uniforms->Get(uniformName);

				return { uniformData->offset };
			}
			else
			{
				auto uniformBlocks = m_Shader.GetUniformBlocks(Shader::Stage::Vertex);
				auto uniformBlockData = uniformBlocks->Get(uniformBlockName);
				auto uniformData = uniformBlockData->uniforms.Get(uniformName);

				return { uniformData->offset };
			}
		}
		case Shader::Stage::Fragment:
		{
			// If no uniform block was specified the uniform is in global space
			if (uniformBlockName.empty())
			{
				auto uniforms = m_Shader.GetUniforms(Shader::Stage::Fragment);
				auto uniformData = uniforms->Get(uniformName);

				return { uniformData->offset };
			}
			else
			{
				auto uniformBlocks = m_Shader.GetUniformBlocks(Shader::Stage::Fragment);
				auto uniformBlockData = uniformBlocks->Get(uniformBlockName);
				auto uniformData = uniformBlockData->uniforms.Get(uniformName);

				return { uniformData->offset };
			}
		}
		case Shader::Stage::TesselationControl:
		case Shader::Stage::TesselationEvaluation:
		case Shader::Stage::Geometry:
		case Shader::Stage::Compute:
			RAY_ASSERT(false, "[UniformAccess] Compute shader stage not supported.");
			break;
		}
	}
}  // namespace At0::Ray
