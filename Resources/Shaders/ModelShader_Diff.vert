#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outFragPos;
layout(location = 2) out vec2 outTexCoord;
layout(location = 3) out vec3 outViewPos;


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
	outNormal = mat3(transpose(inverse(ubo.model))) * inNormal;
	// outNormal = vec3(0.0f, 1.0f, 1.0f);
	outFragPos = vec3(ubo.model * vec4(inPos, 1.0f));
	outTexCoord = inTexCoord;
	outViewPos = camUBO.pos;

	gl_Position = camUBO.proj * camUBO.view * ubo.model * vec4(inPos, 1.0f);
}