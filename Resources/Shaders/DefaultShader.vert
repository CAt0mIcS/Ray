#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;


layout(set = 0, binding = 0) uniform Transforms
{
	mat4 model;
} ubo;


// layout(set = 1, binding = 1) uniform Camera
// {
// 	mat4 view;
// 	mat4 proj;
// } camUBO;


void main()
{
	gl_Position = ubo.model * vec4(inPos, 1.0f);
}