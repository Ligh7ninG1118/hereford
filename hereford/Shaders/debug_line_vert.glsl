#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in float inPerc;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float Perc;

void main()
{
    Perc = inPerc;
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}