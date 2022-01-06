#version 450 core
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

layout(push_constant) uniform Model
{
	vec2 scale;
	vec2 translate;
} pModel;

void main()
{
	uScene.View;
	uScene.Proj;
	uObj.Model;
	pModel.scale;
	pModel.translate;
	
	// gl_Position = vec4(inPos.xy * pModel.scale + pModel.translate, 0.f, 1.f);
	
	gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos.xy, 0.f, 1.f);
}