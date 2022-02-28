#version 450

layout (location = 0) in vec3 inPos;

layout(set = 0, binding = 0) uniform PerSceneData
{
	mat4 View;
	mat4 Proj;
} uScene;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 Model;
} uObj;

layout (set = 1, binding = 2) uniform UBO 
{
	mat4 depthMVP;
} ubo;

 
void main()
{
	uScene.View;
	uScene.Proj;
	uObj.Model;

	gl_Position =  ubo.depthMVP * vec4(inPos, 1.0);
}