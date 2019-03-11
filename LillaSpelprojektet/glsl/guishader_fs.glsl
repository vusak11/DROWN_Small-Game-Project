#version 440 core

in vec2 tex_coords;
out vec4 frag_color;

uniform sampler2D texture_1;
uniform vec2 marker_pos;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture_1, tex_coords).a);
	// Fragments that belong to the mini map portion of the GUI
	if (gl_FragCoord.x >= 1000.0f && gl_FragCoord.y <= 300) {
		// Only render part of mini map within a radius around the map marker
		if (pow(gl_FragCoord.x - marker_pos.x, 2) + pow(gl_FragCoord.y - marker_pos.y, 2) > pow(35, 2)) {
			discard;
		}
		else {
			frag_color = texture(texture_1, tex_coords) * sampled;
		}
	}
	// Fragments that belong to the rest of the GUI
	else {
		frag_color = texture(texture_1, tex_coords) * sampled;
	}
}