#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragc;

layout(set = 1, binding = 0) uniform Region
{
	uint region;
} uRegion;

layout(set = 1, binding = 1) uniform sampler2D src_texture;

float downsample(vec2 uv, vec2 texsz)
{
	float v =
		texture(src_texture, uv + vec2(0.0f, 0.0f) * texsz).x * 0.25f +
		texture(src_texture, uv + vec2(0.0f, 1.0f) * texsz).x * 0.25f +
		texture(src_texture, uv + vec2(1.0f, 0.0f) * texsz).x * 0.25f +
		texture(src_texture, uv + vec2(1.0f, 1.0f) * texsz).x * 0.25f;
	return v;
}

void main()
{
	const vec2 texsz = 1.0f / vec2(2048 , 512); // VE_FONTCACHE_GLYPHDRAW_BUFFER_WIDTH/HEIGHT

	if ( uRegion.region == 0u || uRegion.region == 1u || uRegion.region == 2u )
	{
		float v =
			downsample(uv + vec2(-1.5f, -1.5f) * texsz, texsz) * 0.25f +
			downsample(uv + vec2( 0.5f, -1.5f) * texsz, texsz) * 0.25f +
			downsample(uv + vec2(-1.5f,  0.5f) * texsz, texsz) * 0.25f +
			downsample(uv + vec2( 0.5f,  0.5f) * texsz, texsz) * 0.25f;
		fragc = vec4(1.0f, 1.0f, 1.0f, v);
	}
	else
		fragc = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}