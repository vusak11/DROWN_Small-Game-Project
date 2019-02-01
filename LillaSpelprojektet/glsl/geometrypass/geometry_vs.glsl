#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coordinates;
layout(location = 2) in vec3 normal;

out VS_DATA{
	vec2 TexCoords;
	vec3 Normal;
} VS_OUT;

void main() {
	VS_OUT.TexCoords = tex_coordinates;
	VS_OUT.Normal = normal;
	gl_Position = vec4(position, 1.0f);
}
