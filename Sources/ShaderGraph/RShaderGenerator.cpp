#include "Rpch.h"
#include "RShaderGenerator.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	void ShaderGenerator::Connect(Connection connectionPoint, Scope<Technique> technique)
	{
		m_Connections[connectionPoint] = std::move(technique);
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

void main()
{
	{6}
})";

		std::string fragmentShaderCode = R"(#version 450 core
#extension GL_ARB_separate_shader_objects : enable

{0}

layout(location = 0) out vec4 outColor;

{1}

{2}


void main()
{
	{3}
}
)";

		return { vertexShaderCode, fragmentShaderCode };
	}
}  // namespace At0::Ray
