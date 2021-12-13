#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 rotation;

out vec3 FragPos;
out vec3 Normal;
out float Distance;


void main()
{
    gl_Position = projection * view * model* vec4(aPos, 1.0);
    Distance = gl_Position.z;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal =  vec3(rotation * vec4(aNormal, 1.0));//problem
   
}