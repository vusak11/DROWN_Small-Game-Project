#version 440 core

out vec4 frag_color;

in GS_DATA{
	vec2 tex_coordinates;
} GS_IN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main() {
	frag_color.rgb = texture(texture_diffuse, GS_IN.tex_coordinates).rgb;
	frag_color.a = texture(texture_specular, GS_IN.tex_coordinates).a;
}