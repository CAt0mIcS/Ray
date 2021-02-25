#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 outColor;

layout(binding = 0, set = 0) uniform Transforms
{
	mat4 model;
} ubo;


layout(binding = 1, set = 1) uniform Camera
{
	mat4 view;
	mat4 proj;
} camUBO;


void main()
{
	mat4 mvp = camUBO.proj * camUBO.view * ubo.model;
	
	// gl_Position = ubo.model * vec4(inPos, 1.0f);

	gl_Position = mvp * vec4(inPos, 1.0f);

	outColor = inColor;
}