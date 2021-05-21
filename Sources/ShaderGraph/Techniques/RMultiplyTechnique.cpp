#include "Rpch.h"
#include "RMultiplyTechnique.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	std::string MultiplyTechnique::GetFunctions() const
	{
		std::string functions;
		for (const auto& [connectionPoint, technique] : m_ChildTechniques)
		{
			std::string childFunctions = technique->GetFunctions();
			if (!String::Contains(functions, childFunctions))
				functions += childFunctions + '\n';
		}


		functions +=
			"vec4 MultiplyTechnique(vec4 MultiplyTechnique_left, vec4 MultiplyTechnique_right) { "
			"return MultiplyTechnique_left * MultiplyTechnique_right; }";

		return functions;
	}
	std::string MultiplyTechnique::GetFunctionCalls() const
	{
		return String::Serialize("MultiplyTechnique({0}, {1})",
			m_ChildTechniques.at(MultiplyTechnique::Left)->GetFunctionCalls(),
			m_ChildTechniques.at(MultiplyTechnique::Right)->GetFunctionCalls());
	}
}  // namespace At0::Ray
