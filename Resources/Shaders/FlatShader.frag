#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) out vec4 outColor;


layout(set = 2, binding = 2) uniform Color
{
	vec3 color;
} uCol;


void main()
{
	outColor = vec4(uCol.color, 1.0f);
}
