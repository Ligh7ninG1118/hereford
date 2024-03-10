#version 460 core
layout (location = 0) in vec3 inPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 inColor;

out vec3 outColor;

void main()
{
    gl_Position = projection * view * model * vec4(inPos, 1.0);
    outColor = inColor;
}