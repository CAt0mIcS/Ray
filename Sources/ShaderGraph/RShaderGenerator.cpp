#include "Rpch.h"
#include "RShaderGenerator.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	void ShaderGenerator::Connect(Connection connectionPoint, Scope<Technique> technique)
	{
		m_Connections[connectionPoint] = std::move(technique);
	}

	std::vector<std::string> ShaderGenerator::Generate() const
	{
		std::string vertexShader = R"(#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;

layout(set = 0, binding = 0) uniform PerSceneData
{
	mat4 View;
	mat4 Proj;
} uScene;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 Model;
} uObj;

void main()
{
	gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f);
}
)";

		std::string fragmentShader = String::Serialize(R"(#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) out vec4 outColor;


void main()
{
	{0}
}
)",
			m_Connections.at(Connection::Color)->GetMainShaderCode());

		return { vertexShader, fragmentShader };
	}
}  // namespace At0::Ray
