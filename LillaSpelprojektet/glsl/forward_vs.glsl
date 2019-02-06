#version 440 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_coordinates;

uniform mat4 matrix;

out VS_DATA{
	vec2 tex_coordinates;
} VS_OUT;

void main() {
	VS_OUT.tex_coordinates = a_coordinates;
	gl_Position = matrix * vec4(a_position, 1.0);
}