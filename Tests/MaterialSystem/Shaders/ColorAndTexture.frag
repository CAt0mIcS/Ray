#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform Shading
{
	vec4 color;
} uShading;

layout(set = 1, binding = 3) uniform sampler2D samplerDiffuse;

layout(push_constant) uniform Constants
{
	bool useColor;
	bool useTexture;
} uConstants;

void main()
{
	if(uConstants.useColor && uConstants.useTexture)
		outColor = texture(samplerDiffuse, inUV) * uShading.color;
	else if(uConstants.useTexture)
		outColor = texture(samplerDiffuse, inUV);
	else if(uConstants.useColor)
		outColor = uShading.color;
	else
		outColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

//	outColor = texture(samplerDiffuse, inUV) * uShading.color;
}
