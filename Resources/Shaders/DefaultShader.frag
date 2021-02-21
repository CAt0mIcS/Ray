#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inColor;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D samplerTex;

void main()
{
	outColor = texture(samplerTex, vec2(1.0f, 1.0f));
}
