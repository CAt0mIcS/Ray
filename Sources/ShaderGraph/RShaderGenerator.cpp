#include "Rpch.h"
#include "RShaderGenerator.h"

#include "Utils/RString.h"
#include "Utils/RAssert.h"

#include "Core/RDynamicVertex.h"


namespace At0::Ray
{
	std::vector<std::string> ShaderGenerator::Generate() const
	{
		std::vector shaderTemplates = GetShaderTemplates();
		std::string mergedAttribs;

		uint32_t location = 0;
		uint32_t binding = 2;

		shaderTemplates[1] = String::Serialize(shaderTemplates[1], MergeAttributes(location),
			MergeUniforms(binding), "", GetFunctions(), "outColor = " + GetFunctionCalls() + ";");

		shaderTemplates[0] = GenerateVertexShader(shaderTemplates[0]);

		return shaderTemplates;
	}

	std::string ShaderGenerator::GenerateVertexShader(std::string shaderTemplate) const
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

	std::vector<std::string> ShaderGenerator::GetShaderTemplates() const
	{
		std::string vertexShaderCode = R"(#version 450 core
#extension GL_ARB_separate_shader_objects : enable

{0}
{1}

layout(set = 0, binding = 0) uniform PerSceneData
{
	mat4 View;
	mat4 Proj;
	{2}
} uScene;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 Model;
	{3}
} uObj;

{4}

{5}

{6}

void main()
{
	{7}
})";

		std::string fragmentShaderCode = R"(#version 450 core
#extension GL_ARB_separate_shader_objects : enable

{0}

layout(location = 0) out vec4 outColor;

{1}

{2}

{3}


void main()
{
	{4}
}
)";

		return { vertexShaderCode, fragmentShaderCode };
	}
}  // namespace At0::Ray
