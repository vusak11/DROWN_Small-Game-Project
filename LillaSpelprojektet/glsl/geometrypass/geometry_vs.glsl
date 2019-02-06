#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coordinates;
layout(location = 2) in vec3 normal;

out VS_DATA{
	vec4 world_pos;
	vec2 tex_coords;
	vec3 normal;
} VS_OUT;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	VS_OUT.world_pos = model * vec4(position, 1.0f);

	VS_OUT.tex_coords = tex_coordinates;

	mat3 normal_matrix = mat3(transpose(inverse(model)));
	vec3 normal_ = normalize(normal_matrix * normal);
	VS_OUT.normal = normal_;

	gl_Position = projection * view * model * vec4(position, 1.0f);

}
