#version 440 core

in vec2 tex_coords;
out vec4 frag_color;

uniform sampler2D texture_1;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture_1, tex_coords).a);
	frag_color = texture(texture_1, tex_coords) * sampled;
}