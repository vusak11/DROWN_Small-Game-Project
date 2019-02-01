#version 440

//	Will be used later instead of frag_color
//	in order to apply post-processing effects
//layout(location = 0) out vec3 screenTexture;

out vec4 fragment_color;

in vec4 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

//get light data via uniforms here

uniform vec3 viewPos;



void main() {
	// fetch data from G-buffer
	vec3 fragment_position = texture(gPosition, TexCoords).rgb;
	vec3 normal = texture(gNormal, TexCoords).rgb;
	vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
	float specular = texture(gAlbedoSpec, TexCoords).a;

	// calculate lighting
	/*vec3 lighting = albedo * 0.1; // hard-coded ambient component
	vec3 view_dir = normalize(viewPos - fragment_position);
	for (int i = 0; i < nr_of_lights; i++) {
		vec3 light_direction = normalize(lights[i].position_ - fragment_position);
		vec3 diffuse = max(dot(normal, light_direction), 0.0) * albedo * lights[i].color_;
		lighting += diffuse;
	}*/

	fragment_color = vec4(lighting, 1.0);
}