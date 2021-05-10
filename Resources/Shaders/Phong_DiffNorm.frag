#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inFragPos;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inViewPos;

layout(location = 0) out vec4 outColor;

float lightAmbient = 0.1f;
float lightDiffuse = 5.0f;
float lightSpecular = 1.0f;
vec3 lightPosition = vec3(0.0f, 0.0f, 0.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

layout(set = 2, binding = 2) uniform sampler2D materialDiffuse;
float materialShininess = 1.0f;
vec3 materialSpecularColor = vec3(1.0f, 1.0f, 1.0f);
float specularStrength = 0.5f;

layout(set = 3, binding = 3) uniform sampler2D materialNormal;


void main()
{
    vec3 normal = texture(materialNormal, inTexCoord).rgb;
    normal = normalize(normal * 2.0f - 1.0f);

    // ambient
    vec3 ambient = lightAmbient * texture(materialDiffuse, inTexCoord).rgb;

    // diffuse 
    vec3 lightDir = normalize(lightPosition - inFragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = lightDiffuse * diff * texture(materialDiffuse, inTexCoord).rgb * lightColor;
    
    // specular (no map)
    vec3 viewDir = normalize(inViewPos - inFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    outColor = vec4(ambient + diffuse + specular, 1.0f);
}
