#include "Rpch.h"
#include "RVertexShaderGenerator.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	std::string VertexShaderGenerator::Generate(std::vector<Ref<Node>> rootNodes)
	{
		std::string attributes;
		std::string uniforms;
		std::string functions;
		std::string mainCode;

		uint32_t inputLocation = 0;
		uint32_t outputLocation = 0;
		for (const Ref<Node>& rootNode : rootNodes)
		{
			auto attribs = rootNode->GetAttributes();
			for (const auto& [attribName, attribData] : attribs)
			{
				if (attribData.inOut == "in")
					attributes += String::Serialize("layout(location = {0}) in {1} {2};\n",
						inputLocation++, attribData.type, attribName);
				else if (attribData.inOut == "out")
					attributes += String::Serialize("layout(location = {0}) out {1} {2};\n",
						outputLocation++, attribData.type, attribName);
			}

			mainCode += rootNode->GetFunctionCalls();
		}

		return String::Serialize(GenerateTemplate(), attributes, uniforms, functions, mainCode);
	}

	std::string VertexShaderGenerator::GenerateTemplate() const
	{
		return R"(#version 450 core
#extension GL_ARB_separate_shader_objects : enable

{0}

{1}

{2}

void main()
{
	{3}
}
)";
	}
}  // namespace At0::Ray
