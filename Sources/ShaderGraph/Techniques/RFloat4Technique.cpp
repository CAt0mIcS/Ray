#include "Rpch.h"
#include "RFloat4Technique.h"

#include "Utils/RAssert.h"
#include "Utils/RString.h"


namespace At0::Ray
{
	Float4Technique::Float4Technique(
		Float4 defaultValue, const std::string& uniformBlockName, const std::string& uniformName)
		: m_Value(std::move(defaultValue)), m_UniformBlockName(uniformBlockName),
		  m_UniformName(uniformName)
	{
		if (!uniformBlockName.empty())
		{
			RAY_MEXPECTS(!uniformName.empty(),
				"[Float4Technique] If uniform block name \"{0}\" is given, uniform name is also "
				"required",
				uniformBlockName);

			RequiresBufferUniform(m_UniformBlockName, "vec4", m_UniformName);
		}
	}

	std::string Float4Technique::GetFunctionCalls() const
	{
		if (!m_UniformBlockName.empty() && HasUniformInBlock(m_UniformBlockName, m_UniformName))
			return 'u' + m_UniformBlockName + '.' + m_UniformName;

		return String::Serialize(
			"vec4({0}, {1}, {2}, {3})", m_Value.x, m_Value.y, m_Value.z, m_Value.w);
	}
}  // namespace At0::Ray
