#version 440 core
layout(location = 0) out vec3 geometry_position;
layout(location = 1) out vec3 geometry_normal;
layout(location = 2) out vec4 geometry_albedo_specular;

in GS_DATA{
	vec3 frag_pos;
	vec2 tex_coords;
	vec3 normal;
} GS_IN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular;

void main() {
	const float f_range_1 = 0.0f;
	const float f_range_2 = 0.1f;
	const float f_range_3 = 0.2f;
	float f_scale = GS_IN.frag_pos.z / 255.0f * 2;

	geometry_position = GS_IN.frag_pos;
	geometry_normal = GS_IN.normal;


	if (f_scale <= f_range_1) {
		geometry_albedo_specular.rgb = texture(texture_diffuse1, GS_IN.tex_coords).rgb;
		geometry_albedo_specular.a = 0.0f;
	}
	else if (f_scale < f_range_3) {
		f_scale -= f_range_1;
		f_scale /= (f_range_2 - f_range_1);

		float f_scale_2 = f_scale;
		f_scale = 1.0f - f_scale;

		geometry_albedo_specular.rgb = texture(texture_diffuse1, GS_IN.tex_coords).rgb * f_scale + texture(texture_diffuse, GS_IN.tex_coords).rgb * f_scale_2;
		geometry_albedo_specular.a = texture(texture_specular, GS_IN.tex_coords).a * 0.5;
	}
	else {
		geometry_albedo_specular.rgb = texture(texture_diffuse, GS_IN.tex_coords).rgb;
		geometry_albedo_specular.a = texture(texture_specular, GS_IN.tex_coords).a;
	}

	//geometry_albedo_specular.rgb = texture(texture_diffuse1, GS_IN.tex_coords).rgb;
	//geometry_albedo_specular.a = texture(texture_specular, GS_IN.tex_coords).a;
}