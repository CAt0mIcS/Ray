#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inViewVec;
layout (location = 3) in vec3 inTangent;

layout (location = 0) out vec4 outColor;

layout (set = 2, binding = 2) uniform sampler2D materialDiffuse;
layout (set = 3, binding = 3) uniform sampler2D materialNormal;

vec3 lightVec = vec3(0.0, 2.5, 0.0);

void main()
{
    vec4 color = texture(materialDiffuse, inUV);
	
	vec3 N = normalize(inNormal);
	vec3 T = normalize(inTangent.xyz);
	vec3 B = cross(inNormal, inTangent.xyz);
	mat3 TBN = mat3(T, B, N);
	N = TBN * normalize(texture(materialNormal, inUV).xyz * 2.0 - vec3(1.0));
	
	const float ambient = 0.1;
	vec3 L = normalize(lightVec);
	vec3 V = normalize(inViewVec);
	vec3 R = reflect(-L, N);
	vec3 diffuse = max(dot(N, L), ambient).rrr;
	float specular = pow(max(dot(R, V), 0.0), 32.0);
	outColor = vec4(diffuse * color.rgb + specular, color.a);
}
