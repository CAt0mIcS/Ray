#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragc;

layout(set = 1, binding = 0) uniform sampler2D src_texture;

layout(set = 1, binding = 1) uniform Downsample
{
	uint downsample;
	vec4 colour;
} uData;

void main()
{
	float v = texture(src_texture, uv).x;
	if (uData.downsample == 1u)
	{
		const vec2 texsz = 1.0f / vec2(2048 , 512); // VE_FONTCACHE_GLYPHDRAW_BUFFER_WIDTH/HEIGHT
		v =
			texture(src_texture, uv + vec2(-0.5f,-0.5f) * texsz).x * 0.25f +
			texture(src_texture, uv + vec2(-0.5f, 0.5f) * texsz).x * 0.25f +
			texture(src_texture, uv + vec2( 0.5f,-0.5f) * texsz).x * 0.25f +
			texture(src_texture, uv + vec2( 0.5f, 0.5f) * texsz).x * 0.25f;
	}
	fragc = vec4(uData.colour.xyz, uData.colour.a * v);
}