#version 450 core
#extension GL_ARB_separate_shader_objects : enable


// layout(location = 0) in vec3 inNormal;
// layout(location = 1) in vec3 inFragPos;
layout(location = 2) in vec3 inTexCoord;

layout(location = 0) out vec4 outColor;

float lightAmbient = 0.1f;
float lightDiffuse = 1.0f;
float lightSpecular = 1.0f;
vec3 lightPosition = vec3(0.0f, 0.0f, 0.0f);

layout(set = 2, binding = 2) uniform sampler2D materialDiffuse;
float materialShininess = 1.0f;
vec3 materialSpecular = vec3(1.0f, 1.0f, 1.0f);


void main()
{
    // ambient
    // vec3 ambient = lightAmbient * texture(materialDiffuse, inTexCoord).rgb;

    // diffuse 
    // vec3 norm = normalize(inNormal);
    // vec3 lightDir = normalize(lightPosition - inFragPos);
    // float diff = max(dot(norm, lightDir), 0.0f);
    // vec3 diffuse = lightDiffuse * diff * texture(materialDiffuse, inTexCoord).rgb;

    // specular
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    
    // outColor = vec4(ambient + diffuse, 1.0f);
    outColor = texture(materialDiffuse, inTexCoord.xy);
}
