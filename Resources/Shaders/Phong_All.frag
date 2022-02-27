#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPosWorld;
layout(location = 1) in vec3 inNormalWorld;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform Shading
{
	vec4 color;
	vec4 ambientLightColor; // w is intensity

	vec4 lightColor[10];
	vec3 lightPosition[10];

	uint numLights;
} uShading;


void main()
{
	vec3 diffuseLight = uShading.ambientLightColor.xyz * uShading.ambientLightColor.w;
	vec3 surfaceNormal = normalize(inNormalWorld);

	for(int i = 0; i < uShading.numLights; i++)
	{
		vec4 lightColor = uShading.lightColor[i];
		vec3 lightPosition = uShading.lightPosition[i];

		vec3 directionToLight = lightPosition - inPosWorld;
		float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
		float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
		vec3 intensity = lightColor.xyz * lightColor.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;
	}

	outColor = vec4(diffuseLight * uShading.color.xyz, uShading.color.w);
}