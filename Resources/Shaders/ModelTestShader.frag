#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inFragPos;

layout(location = 0) out vec4 outColor;


float ambientStrength = 0.1;
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);
vec3 lightPos = vec3(0.0f, 1000.0f, 0.0f);


void main()
{
    vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(lightPos - inFragPos);
    
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;
    
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    outColor = vec4(result, 1.0f);
}
