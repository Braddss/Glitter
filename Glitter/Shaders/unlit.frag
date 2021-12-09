#version 400 core


in vec3 pCol;

uniform vec3 unlitColor;


out vec4 FragColor;

void main()
{
    FragColor = (vec4(unlitColor*pCol, 1.0f));
} 