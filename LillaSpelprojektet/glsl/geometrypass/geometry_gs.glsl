#version 440 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_DATA{
	vec2 tex_coords;
	vec3 normal;
}	VS_IN[3];

out GS_DATA{
	vec3 frag_pos;
	vec2 tex_coords;
	vec3 normal;
} GS_OUT;

uniform vec3 view_position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat3 normal_matrix = mat3(transpose(inverse(model)));

	for (int i = 0; i < gl_in.length(); i++) {
		vec3 normal = normalize(normal_matrix * VS_IN[i].normal);
		gl_Position = projection * view * model * gl_in[i].gl_Position;
		GS_OUT.frag_pos = vec3(model * gl_in[i].gl_Position);
		GS_OUT.tex_coords = VS_IN[i].tex_coords;
		GS_OUT.normal = normal;
		EmitVertex();
	}

	EndPrimitive();

}
