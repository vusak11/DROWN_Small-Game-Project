#version 440 core

in vec2 tex_coords;
out vec4 frag_color;

uniform sampler2D texture_1;

void main()
{
	frag_color = texture(texture_1, tex_coords);
}