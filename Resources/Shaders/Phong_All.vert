#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outPosWorld;
layout(location = 1) out vec3 outNormalWorld;


layout(set = 0, binding = 0) uniform PerSceneData
{
	mat4 View;
	mat4 Proj;
	vec3 ViewPos;
} uScene;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 Model;
	mat4 NormalMatrix;
} uObj;

void main()
{
	vec4 positionWorld = uObj.Model * vec4(inPos, 1.f);
	
	gl_Position = uScene.Proj * uScene.View * positionWorld;

	outNormalWorld = normalize(mat3(uObj.NormalMatrix) * inNormal);
	outPosWorld = positionWorld.xyz;
}