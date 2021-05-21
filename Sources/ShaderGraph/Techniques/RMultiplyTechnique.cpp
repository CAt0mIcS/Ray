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


		// Add function for multiple possible data types
		std::string multiplyFunctions;
		const char* dataTypes[] = { "vec4", "vec3", "vec2", "float", "int", "double", "uint" };
		for (uint32_t i = 0; i < std::size(dataTypes); ++i)
		{
			multiplyFunctions +=
				String::Serialize("{0} MultiplyTechnique({0} MultiplyTechnique_left, {0} "
								  "MultiplyTechnique_right) { "
								  "return MultiplyTechnique_left * MultiplyTechnique_right; }\n",
					dataTypes[i]);
		}

		// Remove trailing \n
		multiplyFunctions.erase(multiplyFunctions.end() - 1);
		functions += multiplyFunctions;

		return functions;
	}
	std::string MultiplyTechnique::GetFunctionCalls() const
	{
		return String::Serialize("MultiplyTechnique({0}, {1})",
			m_ChildTechniques.at(MultiplyTechnique::Left)->GetFunctionCalls(),
			m_ChildTechniques.at(MultiplyTechnique::Right)->GetFunctionCalls());
	}
}  // namespace At0::Ray
