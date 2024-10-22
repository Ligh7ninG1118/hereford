#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uiTex;
uniform vec4 uiColor;

void main()
{
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(uiTex, TexCoords).a);
	FragColor = uiColor * sampled;
}