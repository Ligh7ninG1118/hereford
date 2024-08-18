#version 460 core
layout (location = 0) in vec3 inPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = inPos;
	vec4 pos = projection * view * vec4(inPos, 1.0f);
	gl_Position = pos.xyww;
}