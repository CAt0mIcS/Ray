#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inFragPos;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inViewPos;

layout(location = 0) out vec4 outColor;

float lightAmbient = 0.1f;
float lightDiffuse = 5.0f;
float lightSpecular = 1.0f;
// vec3 lightPosition = vec3(0.0f, 0.0f, 0.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

layout(set = 4, binding = 4) uniform Light
{
    vec3 lightPos;
} light;

layout(set = 2, binding = 2) uniform sampler2D materialDiffuse;
layout(set = 3, binding = 3) uniform sampler2D materialSpecular;
float materialShininess = 1.0f;
vec3 materialSpecularColor = vec3(1.0f, 1.0f, 1.0f);

float specularStrength = 0.5f;


void main()
{
    // ambient
    vec3 ambient = lightAmbient * texture(materialDiffuse, inTexCoord).rgb;

    // diffuse 
    vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(light.lightPos - inFragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = lightDiffuse * diff * texture(materialDiffuse, inTexCoord).rgb * lightColor;

    // specular (map)
    vec3 viewDir = normalize(inViewPos - inFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), materialShininess);
    vec3 specular = lightSpecular * spec * texture(materialSpecular, inTexCoord).rgb;

    outColor = vec4(ambient + diffuse + specular, 1.0f);
}
