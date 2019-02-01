#version 440
layout(location = 0) out vec3 geometry_position;
layout(location = 1) out vec3 geometry_normal;
layout(location = 2) out vec4 geometry_albedo_specular;

in GS_DATA{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 Normal;
} GS_IN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main() {
	geometry_position = GS_IN.FragPos;
	geometry_normal = GS_IN.Normal;
	geometry_albedo_specular.rgb = texture(texture_diffuse, GS_IN.TexCoords).rgb;
	geometry_albedo_specular.a = texture(texture_diffuse, GS_IN.TexCoords).a;
}