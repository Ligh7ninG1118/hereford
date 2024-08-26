#version 460 core

layout (location = 0) in vec4 inVertex;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(inVertex.xy, 0.0f, 1.0f);
	TexCoords = inVertex.zw;
}