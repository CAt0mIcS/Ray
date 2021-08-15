#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outFragColor;


layout(set = 0, binding = 0) uniform PerSceneData
{
	mat4 View;
	mat4 Proj;
	vec3 ViewPos;
} uScene;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 Model;
} uObj;


const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0f, -3.0f, -1.0f));
const float AMBIENT = 0.02f;


void main()
{
	gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f);

	vec3 normalWorldSpace = normalize(mat3(uObj.Model) * inNormal);
	float lightIntensity = AMBIENT + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);

	outFragColor = vec3(lightIntensity)/* * color*/;
}