#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vpos;
layout(location = 1) in vec2 vtex;
layout(location = 0) out vec2 uv;

void main()
{
	uv = vtex;
	gl_Position = vec4(vpos.xy * 2.0f - 1.0f, 0.0, 1.0);
}
