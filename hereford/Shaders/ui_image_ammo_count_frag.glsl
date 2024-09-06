#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uiTex;
uniform vec4 uiColor1;
uniform vec4 uiColor2;
uniform float threshold;

void main()
{
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(uiTex, TexCoords).a);
	vec4 color = vec4(0.0f);
	if(gl_FragCoord.x < threshold)
		color = uiColor1;
	else
		color = uiColor2;
	FragColor = color * sampled;
}