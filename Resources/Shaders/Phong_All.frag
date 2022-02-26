#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPosWorld;
layout(location = 1) in vec3 inNormalWorld;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform Shading
{
	vec4 color;
	vec4 ambientLightColor; // w is intensity
	vec3 lightPosition;
	vec4 lightColor;
} uShading;


void main()
{
	vec3 directionToLight = uShading.lightPosition - inPosWorld;
	float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
	
	vec3 lightColor = uShading.lightColor.xyz * uShading.lightColor.w * attenuation;
	vec3 ambientLight = uShading.ambientLightColor.xyz * uShading.ambientLightColor.w;
	vec3 diffuseLight = lightColor * max(dot(normalize(inNormalWorld), normalize(directionToLight)), 0);
	outColor = vec4((diffuseLight + ambientLight) * uShading.color.xyz, uShading.color.w);
}