#version 440 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_DATA{
	vec4 world_pos;
	vec2 tex_coords;
	vec3 normal;
}	VS_IN[3];

out GS_DATA{
	vec3 frag_pos;
	vec2 tex_coords;
	vec3 normal;
} GS_OUT;

void main() {

	for (int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		GS_OUT.frag_pos = vec3(VS_IN[i].world_pos).xyz;
		GS_OUT.tex_coords = VS_IN[i].tex_coords;
		GS_OUT.normal = VS_IN[i].normal;
		EmitVertex();
	}

	EndPrimitive();
}
