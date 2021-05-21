#include "Rpch.h"
#include "RColorTechnique.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	ColorTechnique::ColorTechnique(Float4 defaultColor) : m_Color(std::move(defaultColor)) {}

	std::string ColorTechnique::GetFunctions() const
	{
		return "vec4 ColorTechnique(vec4 ColorTechnique_Color) { return ColorTechnique_Color; }";
	}

	std::string ColorTechnique::GetFunctionCalls() const
	{
		return String::Serialize(
			"ColorTechnique(vec4({0}, {1}, {2}, {3}))", m_Color.x, m_Color.y, m_Color.z, m_Color.w);
	}
}  // namespace At0::Ray
