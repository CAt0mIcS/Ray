#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal; // Required because we need matching inputs in Offscreen.vert and ShadowMapping.vert

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

out gl_PerVertex
{
    vec4 gl_Position;
};

 
void main()
{
	uScene.View;
	uScene.Proj;
	uObj.Model;

	inNormal;

	gl_Position =  ubo.depthMVP * uObj.Model * vec4(inPos, 1.0);
}