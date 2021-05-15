#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform sampler2D samplerDiffuse;

void main()
{
    outColor = texture(samplerDiffuse, inUV);
}
