#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outFragPos;
layout(location = 2) out vec2 outUV;
layout(location = 3) out vec3 outViewPos;


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

void main()
{
	outNormal = mat3(transpose(inverse(uObj.Model))) * inNormal;
	outFragPos = vec3(uObj.Model * vec4(inPos, 1.0f));
	outUV = inUV;
	outViewPos = UScene.ViewPos;

	gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f);
}