#version 400 core

uniform vec3 lightPos;

in vec3 FragPos;
in vec3 Normal;  


out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);

    float ambientStrength = 0.5f;
    vec3 result = (ambientStrength+diff)*vec3(1.0f, 0.5f, 0.2f);
    FragColor = (vec4(result, 1.0f));
} 