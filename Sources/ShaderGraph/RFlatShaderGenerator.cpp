#include "Rpch.h"
#include "RFlatShaderGenerator.h"

#include "Utils/RString.h"
#include "Core/RDynamicVertex.h"


namespace At0::Ray
{
	std::vector<std::string> FlatShaderGenerator::Generate() const
	{
		std::vector shaderTemplates = GetShaderTemplates();
		std::string mergedAttribs;

		for (const auto& [connection, technique] : m_ChildTechniques)
		{
			uint32_t location = 0;
			uint32_t binding = 2;

			mergedAttribs = MergeAttributes(location);

			shaderTemplates[1] =
				String::Serialize(shaderTemplates[1], mergedAttribs, MergeUniforms(binding), "",
					technique->GetFunctions(), "outColor = " + technique->GetFunctionCalls() + ";");

			shaderTemplates[0] = GenerateVertexShader(shaderTemplates[0]);
		}

		return shaderTemplates;
	}

	std::string FlatShaderGenerator::GenerateVertexShader(std::string shaderTemplate) const
	{
		std::string attributes = String::Serialize(
			"layout(location = 0) in vec3 {0};", AttributeMap<AttributeType::Position>::Semantic);

		std::string main =
			"gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f);";

		uint32_t vertexInputLocation = 1;
		uint32_t vertexOutputLocation = 0;
		attributes += MergeVertexAttributes(vertexInputLocation, vertexOutputLocation);
		main += MergeVertexAssignments();

		return String::Serialize(shaderTemplate, attributes, "", "", "", "", "", "", main);
	}
}  // namespace At0::Ray
