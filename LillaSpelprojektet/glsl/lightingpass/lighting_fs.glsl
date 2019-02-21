#version 440

layout(location = 0) out vec3 screen_texture;
in vec2 tex_coords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;


struct Light {
	vec3 position;
	vec3 color;
};
const int nr_of_lights = 1;
uniform Light lights[nr_of_lights];

uniform vec3 light_position;

uniform vec3 viewPos;


void main() {
	// fetch data from G-buffer
	vec3 fragment_position = texture(g_position, tex_coords).rgb;
	vec3 normal = texture(g_normal, tex_coords).rgb;
	vec3 albedo = texture(g_albedo_spec, tex_coords).rgb;
	float specular = texture(g_albedo_spec, tex_coords).a;

	// calculate lighting
	vec3 ambient = 0.15 * albedo;
	vec3 lightDir = normalize(light_position - fragment_position);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * albedo;
	vec3 viewDir = normalize(viewPos - fragment_position);
	float spec = 0.0;

	vec3 reflectDir = reflect(-lightDir, normal);
	spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);

	vec3 speculars = vec3(0.3) * spec;
	screen_texture = vec3(ambient + diffuse /*+ speculars*/);

}