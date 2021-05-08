#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;

layout(location = 0) out vec3 outUVW;

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
	camUBO.view;
	gl_Position = camUBO.proj * ubo.model * vec4(inPos, 1.0f);
	outUVW = inPos;
	
	// Convert cubemap coordinates into Vulkan coordinate space
	outUVW.xy *= -1.0f;
}