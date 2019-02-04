#version 440
layout(location = 0) out vec3 geometry_position;
layout(location = 1) out vec3 geometry_normal;
layout(location = 2) out vec4 geometry_albedo_specular;

in GS_DATA{
	vec3 frag_pos;
	vec2 tex_coords;
	vec3 normal;
} GS_IN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main() {
	geometry_position = GS_IN.frag_pos;
	geometry_normal = GS_IN.normal;
	geometry_albedo_specular.rgb = texture(texture_diffuse, GS_IN.tex_coords).rgb;
	geometry_albedo_specular.a = texture(texture_diffuse, GS_IN.tex_coords).a;
}