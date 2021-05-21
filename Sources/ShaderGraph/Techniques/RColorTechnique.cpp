#include "Rpch.h"
#include "RColorTechnique.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	ColorTechnique::ColorTechnique(Float4 defaultColor) : m_Color(std::move(defaultColor)) {}

	std::string ColorTechnique::GetInputAttributes() const { return ""; }

	std::string ColorTechnique::GetUniforms() const { return ""; }

	std::string ColorTechnique::GetFunctions() const { return ""; }

	std::string ColorTechnique::GetMainShaderCode() const
	{
		return String::Serialize(
			"outColor = vec4({0}, {1}, {2}, {3});", m_Color.x, m_Color.y, m_Color.z, m_Color.w);
	}
}  // namespace At0::Ray
