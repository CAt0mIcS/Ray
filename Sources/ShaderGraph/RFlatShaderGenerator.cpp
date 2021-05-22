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
		}

		shaderTemplates[0] = GenerateVertexShader(shaderTemplates[0], mergedAttribs);

		return shaderTemplates;
	}

	std::string FlatShaderGenerator::GenerateVertexShader(
		std::string shaderTemplate, std::string fragmentShaderInputs) const
	{
		std::string attributes = String::Serialize(
			"layout(location = 0) in vec3 {0};", AttributeMap<AttributeType::Position>::Semantic);

		std::string main =
			"gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f);";

		AddAttributes(fragmentShaderInputs, attributes, main);

		return String::Serialize(shaderTemplate, attributes, "", "", "", "", "", "", main);
	}

	void FlatShaderGenerator::AddAttributes(
		std::string fragmentAttributes, std::string& attributes, std::string& main)
	{
		// Maps input attribute name to output attribute name
		std::unordered_map<std::string, std::string> inputToOutput;

		// Increment location by 1 to be able to take position at location 0
		std::string vertexInputs;
		{
			std::istringstream iss(fragmentAttributes);

			uint32_t i = 0;
			for (std::string line; std::getline(iss, line); ++i)
			{
				std::string currentLocation = String::Serialize("location = {0}", i);
				std::string nextLocation = String::Serialize("location = {0}", i + 1);

				if (size_t pos = line.find(currentLocation); pos != std::string::npos)
				{
					line.replace(pos, currentLocation.length(), nextLocation);
					vertexInputs += '\n' + line;

					std::string inputName = line.substr(line.find_last_of(' '));
					inputName.erase(inputName.end() - 1);
					std::string outputName = inputName;
					outputName.replace(outputName.find("in"), strlen("in"), "out");
					inputToOutput[inputName] = outputName;
				}
				else
					--i;
			}
		}

		{
			// Replace in with out, preserve index
			size_t idx = fragmentAttributes.find("in");
			while (idx != std::string::npos)
			{
				fragmentAttributes.replace(idx, strlen("in"), "out");
				idx = fragmentAttributes.find("in");
			}

			attributes += vertexInputs + fragmentAttributes;

			for (const auto& [inputName, outputName] : inputToOutput)
			{
				main += '\n' + outputName + " = " + inputName + ';';
			}
		}
	}
}  // namespace At0::Ray
