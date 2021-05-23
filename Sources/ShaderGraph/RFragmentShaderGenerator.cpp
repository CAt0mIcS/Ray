#include "Rpch.h"
#include "RFragmentShaderGenerator.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	static std::string GetAttributes(const Node& rootNode, uint32_t& inputLocation,
		uint32_t& outputLocation, std::string_view attributes)
	{
		std::string newAttributes;

		auto attribs = rootNode.GetAttributes();
		for (const auto& [attribName, attribData] : attribs)
		{
			if (attributes.find(attribName) != std::string::npos)
				continue;

			if (attribData.inOut == "in")
				newAttributes += String::Serialize("layout(location = {0}) in {1} {2};\n",
					inputLocation++, attribData.type, attribName);
			else if (attribData.inOut == "out")
				newAttributes += String::Serialize("layout(location = {0}) out {1} {2};\n",
					outputLocation++, attribData.type, attribName);
		}

		return newAttributes;
	}

	static std::string GetUniforms(
		const Node& rootNode, uint32_t& binding, std::string_view uniforms)
	{
		std::string newUniforms;

		// Buffer uniforms
		{
			auto bufferUniforms = rootNode.GetBufferUniforms();
			for (const auto& [uBufferName, uniformData] : bufferUniforms)
			{
				if (uniforms.find(uBufferName) != std::string::npos)
					continue;

				std::string uniformsInBlock;

				for (const auto& [uniformType, uniformName] : uniformData)
					uniformsInBlock += '\n' + uniformType + ' ' + uniformName + ';';

				// Predefined set 0 for per scene data uniform
				if (uBufferName == "PerSceneData")
					newUniforms += String::Serialize("layout(set = 0, binding = 0) uniform "
													 "PerSceneData\n{{0}\n} uPerSceneData;\n",
						uniformsInBlock);
				else
					newUniforms += String::Serialize(
						"layout(set = 1, binding = {0}) uniform {1}\n{{2}\n} u{1};\n", binding++,
						uBufferName, uniformsInBlock);
			}
		}

		// Sampler uniforms
		{
			auto samplerUniforms = rootNode.GetSamplerUniforms();
			for (const auto& [samplerName, samplerType] : samplerUniforms)
				if (uniforms.find(samplerName) == std::string::npos)
					newUniforms +=
						String::Serialize("layout(set = 1, binding = {0}) uniform {1} {2};\n",
							binding++, samplerType, samplerName);
		}

		return newUniforms;
	}

	static std::string GetFunctions(const Node& rootNode, std::string_view functions)
	{
		std::string newFunctions;
		auto nodeFunctions = rootNode.GetFunctions();
		for (const std::string& fn : nodeFunctions)
			if (functions.find(fn) == std::string::npos)
				newFunctions += fn;
		return newFunctions;
	}

	std::string FragmentShaderGenerator::Generate(std::vector<Ref<Node>> rootNodes)
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
			attributes += GetAttributes(*rootNode, inputLocation, outputLocation, attributes);
			uniforms += GetUniforms(*rootNode, binding, uniforms);
			functions += GetFunctions(*rootNode, functions);
			mainCode += rootNode->GetFunctionCalls() + ";\n";
		}

		return String::Serialize(GenerateTemplate(), attributes, uniforms, functions, mainCode);
	}

	std::string FragmentShaderGenerator::GenerateTemplate() const
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
