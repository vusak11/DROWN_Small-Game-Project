#version 440 core

out vec4 frag_color;
in vec2 tex_coordinates;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tex_coordinates).r);
	frag_color = vec4(textColor, 1.0) * sampled;
}