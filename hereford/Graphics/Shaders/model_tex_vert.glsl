#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;


void main()
{
    TexCoords = inTexCoords;    
    WorldPos = vec3(model * vec4(inPos, 1.0f));
    Normal = inNormal;
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}