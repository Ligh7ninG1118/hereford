#version 460 core

uniform sampler2D tex_diffuse_1;

in vec2 TexCoords;

out vec4 FragColor;

void main()
{    
	vec3 diffTex = vec3(texture(tex_diffuse_1, TexCoords));

    FragColor = vec4(diffTex, 1.0f);
}

