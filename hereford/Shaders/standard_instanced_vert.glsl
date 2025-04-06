#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inTexCoords;
layout (location = 3) in mat4 inInstanceMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 inColor;

out vec3 outColor;
out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    //WorldPos = vec3(model * vec4(inPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * inNorm;
    TexCoords = inTexCoords;

    gl_Position = projection * view * inInstanceMatrix * vec4(inPos, 1.0);
    outColor = inColor;
}