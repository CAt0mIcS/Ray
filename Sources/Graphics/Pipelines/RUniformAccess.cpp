#include "Rpch.h"
#include "RUniformAccess.h"

#include "RShader.h"
#include "RPipeline.h"


namespace At0::Ray
{
	UniformAccess::UniformAccess(const Pipeline& pipeline) {}

	UniformAccess::ValueType& UniformAccess::Resolve(Shader::Stage stageFlag)
	{
		return m_Uniforms[stageFlag];
	}
}  // namespace At0::Ray
