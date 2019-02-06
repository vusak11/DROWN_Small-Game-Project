#version 440 core

out vec4 frag_color;

in vec2 tex_coordinates;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main() {
	frag_color.rgb = texture(texture_diffuse, tex_coordinates).rgb;
	frag_color.a = texture(texture_specular, tex_coordinates).a;
}