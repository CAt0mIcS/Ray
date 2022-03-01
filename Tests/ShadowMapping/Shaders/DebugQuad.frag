#version 450

layout (set = 1, binding = 2) uniform sampler2D shadowMap;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

float LinearizeDepth(float depth)
{
  float n = 1.0; // camera z near
  float f = 128.0; // camera z far
  float z = depth;
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main() 
{
	float depth = texture(shadowMap, inUV).r;
	outColor = vec4(vec3(1.0-LinearizeDepth(depth)), 1.0);
}