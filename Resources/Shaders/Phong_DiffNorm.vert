#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outViewVec;
layout (location = 3) out vec3 outTangent;


layout(set = 0, binding = 0) uniform Camera
{
	mat4 view;
	mat4 proj;
	vec3 pos;
} camUBO;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 model;
} ubo;

void main()
{
	outNormal = inNormal;
	outUV = inUV;
	outTangent = inTangent;
	gl_Position = camUBO.proj * camUBO.view * ubo.model * vec4(inPos, 1.0f);

	outNormal = mat3(ubo.model) * inNormal;
	vec4 pos = ubo.model * vec4(inPos, 1.0);
	outViewVec = camUBO.pos - pos.xyz;
}