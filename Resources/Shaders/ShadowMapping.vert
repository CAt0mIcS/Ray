#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

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

layout (set = 1, binding = 2) uniform Shading
{
	mat4 lightSpace;
	vec4 lightPosition[1];
} uShading;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outViewVec;
layout (location = 2) out vec3 outLightVec;
layout (location = 3) out vec4 outShadowCoord;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 );

void main() 
{
	outNormal = inNormal;

	gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos.xyz, 1.0);
	
    vec4 pos = uObj.Model * vec4(inPos, 1.0);
    outNormal = mat3(uObj.Model) * inNormal;
    outLightVec = normalize(uShading.lightPosition[0].xyz - inPos);
    outViewVec = -pos.xyz;

	outShadowCoord = (biasMat * uShading.lightSpace * uObj.Model) * vec4(inPos, 1.0);	
}

