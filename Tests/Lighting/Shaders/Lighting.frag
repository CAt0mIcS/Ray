#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPosWorld;
layout(location = 1) in vec3 inNormalWorld;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform Shading
{
	vec4 color;
	vec4 ambientLightColor; // w is intensity

	vec4 ptLightColor[10];
	vec4 ptLightPos[10];
	uint ptNumLights;

	vec4 dirLightColor[10];
	vec4 dirLightDirection[10];
	uint dirNumLights;

	vec4 spotLightColor[10];
	vec4 spotLightPos[10];
	uint spotNumLights;

} uShading;


void main()
{
	vec3 diffuseLight = uShading.ambientLightColor.xyz * uShading.ambientLightColor.w;
	vec3 surfaceNormal = normalize(inNormalWorld);

	// Point lights
	for(int i = 0; i < uShading.ptNumLights; i++)
	{
		vec4 lightColor = uShading.ptLightColor[i];
		vec3 lightPosition = uShading.ptLightPos[i].xyz;

		vec3 directionToLight = lightPosition - inPosWorld;
		float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared

		float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
		vec3 intensity = lightColor.xyz * lightColor.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;
	}

	// Directional lights
	for(int i = 0; i < uShading.dirNumLights; i++)
	{
		vec4 lightColor = uShading.dirLightColor[i];
		vec3 lightDirection = uShading.dirLightDirection[i].xyz;
	
		vec3 directionToLight = normalize(-lightDirection);
		float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
		
		float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
		vec3 intensity = lightColor.xyz * lightColor.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;
	}

	// Spot lighting
	for(int i = 0; i < uShading.spotNumLights; i++)
	{
		vec4 lightColor = uShading.spotLightColor[i];
		vec3 lightPosition = uShading.spotLightPos[i].xyz;

		vec3 directionToLight = lightPosition - inPosWorld;
		float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
		float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
		vec3 intensity = lightColor.xyz * lightColor.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;
	}

	outColor = vec4(diffuseLight * uShading.color.xyz, uShading.color.w);
}