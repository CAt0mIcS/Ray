#include "Rpch.h"
#include "RFlatShaderGenerator.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	std::vector<std::string> FlatShaderGenerator::Generate() const
	{
		std::vector shaderTemplates = GetShaderTemplates();

		for (const auto& [connection, technique] : m_Connections)
		{
			shaderTemplates[1] =
				String::Serialize(shaderTemplates[1], technique->GetInputAttributes(),
					technique->GetBufferUniforms() + "\n" + technique->GetSamplerUniforms(), "",
					technique->GetFunctions(), "outColor = " + technique->GetFunctionCalls() + ";");
		}

		shaderTemplates[0] = String::Serialize(shaderTemplates[0],
			"layout(location = 0) in vec3 inPos;\nlayout(location = 1) in vec2 inUV;",	// Input
																						// attributes
			"layout(location = 0) out vec2 outUV;",	 // Output attributes
			"",										 // Extra per-scene data
			"",										 // Extra per-object data
			"",										 // Extra uniforms
			"",										 // Constants
			"",										 // Functions
			"gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f); outUV = "
			"inUV;"	 // Main
					 // function
					 // code
		);

		return shaderTemplates;
	}
}  // namespace At0::Ray
