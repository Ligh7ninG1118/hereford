#version 460 core

out vec4 FragColor;

uniform vec4 startColor;
uniform vec4 endColor;

in float Perc;

void main()
{
	FragColor = mix(startColor, endColor, Perc);
}