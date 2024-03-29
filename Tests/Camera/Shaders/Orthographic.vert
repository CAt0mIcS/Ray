#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec2 inPos;

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
	uScene.View;

	gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 0.f, 1.f);
}