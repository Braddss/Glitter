#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 pointColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec2 pCol;

void main()
{
    pCol = pointColor;
    gl_Position = projection * view * model* vec4(aPos, 1.0);
}