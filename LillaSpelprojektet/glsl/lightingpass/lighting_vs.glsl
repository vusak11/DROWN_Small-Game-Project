#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coordinates;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(position, 1.0f);
	TexCoords = tex_coordinates;
}