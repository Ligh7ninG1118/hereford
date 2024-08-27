#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uiTex;
uniform vec3 textColor;
uniform float threshold;

void main()
{
	if(gl_FragCoord.x > threshold)
		return;

	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(uiTex, TexCoords).a);
	FragColor = vec4(textColor, 1.0f) * sampled;
}