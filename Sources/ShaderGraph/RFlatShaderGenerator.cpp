#include "Rpch.h"
#include "RFlatShaderGenerator.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	std::vector<std::string> FlatShaderGenerator::Generate() const
	{
		std::vector shaderTemplates = GetShaderTemplates();

		for (const auto& [connection, technique] : m_ChildTechniques)
		{
			uint32_t location = 0;
			uint32_t binding = 2;
			shaderTemplates[1] = String::Serialize(shaderTemplates[1], MergeAttributes(location),
				MergeUniforms(binding), "", technique->GetFunctions(),
				"outColor = " + technique->GetFunctionCalls() + ";");
		}

		shaderTemplates[0] = String::Serialize(shaderTemplates[0],
			"layout(location = 0) in vec3 inPos;",	// Input attributes
			"",										// Output attributes
			"",										// Extra per-scene data
			"",										// Extra per-object data
			"",										// Extra uniforms
			"",										// Constants
			"",										// Functions
			"gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f);"	 // Main
																						 // function
																						 // code
		);

		return shaderTemplates;
	}
}  // namespace At0::Ray
