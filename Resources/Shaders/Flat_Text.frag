#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform sampler2D samplerTextureAtlas;

// layout(set = 1, binding = 3) uniform FontData
// {
//     ivec2 sizes[];
// } uFont;
// 
// layout(set = 1, binding = 4) uniform TextData
// {
//     uint numCharacters;
// } uText;


void main()
{
    outColor = texture(samplerTextureAtlas, inUV);
}
