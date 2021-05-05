#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) out vec4 outColor;

layout(set = 2, binding = 2) uniform Shading
{
	vec3 color;
} uShading;

void main()
{
	outColor = vec4(uShading.color, 1.0f);
}
