#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coordinates;

out vec2 tex_coords;

void main()
{
	gl_Position = position; // vec4(position, 1.0f);
	tex_coords = tex_coordinates;
}