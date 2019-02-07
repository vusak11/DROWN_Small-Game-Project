#version 440
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_DATA{
	vec2 tex_coordinates;
} VS_IN[3];

out GS_DATA{
	vec2 tex_coordinates;
} GS_OUT;

void main() {

	for (int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		GS_OUT.tex_coordinates = VS_IN[i].tex_coordinates;
		EmitVertex();
	}

	EndPrimitive();
}