#version 400 core


in vec2 pCol;

uniform vec3 unlitColor;


out vec4 FragColor;

void main()
{
    FragColor = (vec4(unlitColor*pCol.x,pCol.y));
} 