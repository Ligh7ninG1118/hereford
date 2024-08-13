#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex_diffuse_1;

void main()
{    
    FragColor = texture(tex_diffuse_1, TexCoords);
}