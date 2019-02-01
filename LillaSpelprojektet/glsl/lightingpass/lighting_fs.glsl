#version 440

layout(location = 0) out vec3 screen_texture;

in vec2 TexCoords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;


struct Light {
	vec3 position;
	vec3 color;
};
const int nr_of_lights = 1;
uniform Light lights[nr_of_lights];

uniform vec3 viewPos;


void main() {
	// fetch data from G-buffer
	vec3 fragment_position = texture(g_position, TexCoords).rgb;
	vec3 normal = texture(g_normal, TexCoords).rgb;
	vec3 albedo = texture(g_albedo_spec, TexCoords).rgb;
	float specular = texture(g_albedo_spec, TexCoords).a;

	// calculate lighting

	vec3 lighting = albedo * 0.1; // hard-coded ambient component
	vec3 view_dir = normalize(viewPos - fragment_position);
	for (int i = 0; i < nr_of_lights; i++) {
		vec3 light_direction = normalize(lights[i].position - fragment_position);
		vec3 diffuse = max(dot(normal, light_direction), 0.0) * albedo * lights[i].color;
		lighting += diffuse;
	}

	screen_texture = lighting;
}