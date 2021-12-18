#version 400 core

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

in vec3 FragPos;
in vec3 Normal;  
in float Distance;

out vec4 FragColor;

void main()
{
    float maxDist = 60;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);

    float ambientStrength = 0.2f;

    vec3 result = (ambientStrength+diff*(1-ambientStrength))*vec3(lightColor)*lightIntensity;


    vec3 fog= vec3(1,1,1)*0.5;
    float scalar =  (Distance/maxDist);
    if(scalar>1)scalar =1;
    result = result +scalar*(fog-result);

    
    FragColor = (vec4(result, 1.0f));
} 