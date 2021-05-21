#include "Rpch.h"
#include "RFloat4Technique.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	Float4Technique::Float4Technique(Float4 defaultValue) : m_Value(std::move(defaultValue)) {}

	std::string Float4Technique::GetFunctionCalls() const
	{
		return String::Serialize(
			"vec4({0}, {1}, {2}, {3})", m_Value.x, m_Value.y, m_Value.z, m_Value.w);
	}
}  // namespace At0::Ray
