#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outUV;
layout(location = 2) out vec3 outViewVec;
layout(location = 3) out vec3 outTangent;
layout(location = 4) out vec3 outLightVec;


layout(set = 0, binding = 0) uniform PerSceneData
{
	mat4 View;
	mat4 Proj;
	vec3 ViewPos;
	vec3 LightPos;
} uScene;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 Model;
} uObj;

void main()
{
	outNormal = inNormal;
	outUV = inUV;
	outTangent = inTangent;
	gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f);

	outNormal = mat3(uObj.Model) * inNormal;
	vec4 pos = uObj.Model * vec4(inPos, 1.0);
	outLightVec = uScene.LightPos - pos.xyz;
	outViewVec = uScene.ViewPos - pos.xyz;
}
