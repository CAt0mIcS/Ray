#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) out vec2 outUV;


// layout(set = 0, binding = 0) uniform PerSceneData
// {
// 	mat4 View;
// 	mat4 Proj;
// } uScene;
// 
// layout(set = 1, binding = 1) uniform PerObjectData
// {
// 	mat4 Model;
// } uObj;

void main()
{
	// uScene.View;
	// uScene.Proj;
	// uObj.Model;

	outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
}