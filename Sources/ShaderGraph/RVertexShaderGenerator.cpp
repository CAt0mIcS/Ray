#include "Rpch.h"
#include "RVertexShaderGenerator.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	static std::string GetAttributes(
		const Node& rootNode, uint32_t& inputLocation, uint32_t& outputLocation)
	{
		std::string attributes;
		auto attribs = rootNode.GetAttributes();
		for (const auto& [attribName, attribData] : attribs)
		{
			if (attribData.inOut == "in")
				attributes += String::Serialize("layout(location = {0}) in {1} {2};\n",
					inputLocation++, attribData.type, attribName);
			else if (attribData.inOut == "out")
				attributes += String::Serialize("layout(location = {0}) out {1} {2};\n",
					outputLocation++, attribData.type, attribName);
		}

		return attributes;
	}

	static std::string GetUniforms(const Node& rootNode, uint32_t& binding)
	{
		std::string uniforms;

		// Buffer uniforms
		{
			auto bufferUniforms = rootNode.GetBufferUniforms();
			for (const auto& [uBufferName, uniformData] : bufferUniforms)
			{
				std::string uniformsInBlock;

				for (const auto& [uniformType, uniformName] : uniformData)
					uniformsInBlock += '\n' + uniformType + ' ' + uniformName + ';';

				// Predefined set 0 for per scene data uniform
				if (uBufferName == "PerSceneData")
					uniforms += String::Serialize("layout(set = 0, binding = 0) uniform "
												  "PerSceneData\n{{0}\n} uPerSceneData;\n",
						uniformsInBlock);
				else
					uniforms += String::Serialize(
						"layout(set = 1, binding = {0}) uniform {1}\n{{2}\n} u{1};\n", binding++,
						uBufferName, uniformsInBlock);
			}
		}

		// Sampler uniforms
		{
			auto samplerUniforms = rootNode.GetSamplerUniforms();
			for (const auto& [samplerName, samplerType] : samplerUniforms)
				uniforms += String::Serialize("layout(set = 1, binding = {0}) uniform {1} {2};\n",
					binding++, samplerType, samplerName);
		}

		return uniforms;
	}

	std::string VertexShaderGenerator::Generate(std::vector<Ref<Node>> rootNodes)
	{
		std::string attributes;
		std::string uniforms;
		std::string functions;
		std::string mainCode;

		uint32_t inputLocation = 0;
		uint32_t outputLocation = 0;
		uint32_t binding = 1;
		for (const Ref<Node>& rootNode : rootNodes)
		{
			attributes += GetAttributes(*rootNode, inputLocation, outputLocation);
			uniforms += GetUniforms(*rootNode, binding);

			auto nodeFunctions = rootNode->GetFunctions();
			functions += std::accumulate(nodeFunctions.begin(), nodeFunctions.end(), std::string{});

			mainCode += rootNode->GetFunctionCalls() + ';';
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
