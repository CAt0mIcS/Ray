#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outFragPos;


layout(set = 0, binding = 0) uniform Camera
{
	mat4 view;
	mat4 proj;
} camUBO;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 model;
} ubo;

void main()
{
	outNormal = mat3(transpose(inverse(ubo.model))) * inNormal;  
	outFragPos = vec3(ubo.model * vec4(inPos, 1.0f));

	gl_Position = camUBO.proj * camUBO.view * ubo.model * vec4(inPos, 1.0f);
}