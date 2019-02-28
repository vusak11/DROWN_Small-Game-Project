#version 440

layout(location = 0) out vec3 screen_texture;
in vec2 tex_coords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;


struct Light {
	vec3 position;
	vec3 color;

	float a_linear;
	float a_quadratic;
	float radius;
};
const int nr_of_lights = 21;
uniform Light lights[nr_of_lights];

uniform vec3 view_pos;


void main() {
	// fetch data from G-buffer
	vec3 fragment_position = texture(g_position, tex_coords).rgb;
	vec3 normal = texture(g_normal, tex_coords).rgb;
	vec3 albedo = texture(g_albedo_spec, tex_coords).rgb;
	float specular = texture(g_albedo_spec, tex_coords).a;

	// calculate lighting
	vec3 lighting = 0.15 * albedo;
	vec3 view_dir = normalize(view_pos - fragment_position);

	for (int i = 0; i < nr_of_lights; ++i) {
		float distance = length(lights[i].position - fragment_position);
		if (distance < lights[i].radius) {
			// diffuse
			vec3 light_dir = normalize(lights[i].position - fragment_position);
			vec3 diffuse = max(dot(normal, light_dir), 0.0) * albedo * lights[i].color;
			//specular
			vec3 halfway_dir = normalize(light_dir + view_dir);
			float spec = pow(max(dot(normal, halfway_dir), 0.0), 8.0);;
			vec3 speculars = lights[i].color * spec * specular;
			//Attenuation
			float attenuation = 1.0 / (1.0 + lights[i].a_linear * distance + lights[i].a_quadratic * distance * distance);
			diffuse *= attenuation;
			speculars *= attenuation;
			lighting += diffuse + speculars;
		}
	}
	screen_texture = lighting;
}