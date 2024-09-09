#version 460 core

layout (location = 0) in vec2 inPos;

out vec2 TexCoords;

void main()
{
	TexCoords = inPos * 0.5f, + 0.5f;
	gl_Position = vec4(inPos, 0.0f, 1.0f);
}