#version 440
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_DATA{
	vec2 TexCoords;
	vec3 Normal;
}	VS_IN[3];

out GS_DATA{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 Normal;
} GS_OUT;

uniform vec3 view_position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat3 normal_matrix = mat3(transpose(inverse(model)));

	for (int i = 0; i < 3; i++) {
		vec3 normal = normalize(normal_matrix * VS_IN[i].Normal);
		gl_Position = projection * view * model * gl_in[i].gl_Position;
		GS_OUT.FragPos = vec3(model * gl_in[i].gl_Position);
		GS_OUT.TexCoords = VS_IN[i].TexCoords;
		GS_OUT.Normal = normal;
		EmitVertex();
	}
	EndPrimitive();
}
