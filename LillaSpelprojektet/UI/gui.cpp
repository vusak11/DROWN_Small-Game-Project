#include "gui.h"

GUI::GUI() {
	health_bar_length_ = (300 / 100) * 100; //Max length of health bar divided by Max HP, multiplied by current HP
	current_slot_1_ = quick_slot_texture_;
	current_slot_2_ = quick_slot_texture_;
}

GUI::~GUI() {

}

void GUI::Initiliaze()
{
	// Load and store textures
	LoadTexture((char*)"../Resources/GUI/healthbar.png", health_bar_texture_);
	LoadTexture((char*)"../Resources/GUI/gui_key.png", key_texture_);
	LoadTexture((char*)"../Resources/GUI/quickslot.png", quick_slot_texture_);
	LoadTexture((char*)"../Resources/GUI/quickslot_sword.png", quick_slot_sword_texture_);
	LoadTexture((char*)"../Resources/GUI/quickslot_axe.png", quick_slot_axe_texture_);
	LoadTexture((char*)"../Resources/GUI/quickslot_dash.png", quick_slot_dash_texture_);
	LoadTexture((char*)"../Resources/GUI/quickslot_dbljump.png", quick_slot_dbljump_texture_);
	LoadTexture((char*)"../Resources/GUI/MiniMap_70_c.png", mini_map_);
	LoadTexture((char*)"../Resources/GUI/map_marker.png", map_marker_);

	//current_slot_1_ = quick_slot_texture_;
	//current_slot_2_ = quick_slot_texture_;

	// CONFIG VAO, VBO
	glGenVertexArrays(1, &vertex_array_object_);
	glGenBuffers(1, &vertex_buffer_object_);
	glBindVertexArray(vertex_array_object_);
	// Update content of VBO memory
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_STATIC_DRAW); // Be sure to use glBufferSubData and not glBufferData

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GUI::RenderHealthBar() {
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vertex_array_object_);
	
	GLfloat bar_vertices[4][4] = {
		// Positions				 // Texture Coords
		{0.0f,  700.0f,				 0.0f, 1.0f},
		{0.0f,  670.0f,				 0.0f, 0.0f},
		{health_bar_length_, 700.0f, 1.0f, 1.0f},
		{health_bar_length_, 670.0f, 1.0f, 0.0f}
	};
	//Render texture on bar
	glBindTexture(GL_TEXTURE_2D, health_bar_texture_);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bar_vertices), &bar_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Render bar
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GUI::RenderGUIObject(float xpos, float ypos, float offset, GLuint slot_texture) {
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vertex_array_object_);

	GLfloat slot_vertices[4][4] = {
		// Positions					// Texture Coords
		{xpos, ypos,					0.0f, 0.0f},
		{xpos, ypos - offset,			0.0f, 1.0f},
		{xpos + offset, ypos,			1.0f, 0.0f},
		{xpos + offset, ypos - offset,	1.0f, 1.0f},
	};
	//Render texture on quad
	glBindTexture(GL_TEXTURE_2D, slot_texture);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(slot_vertices), &slot_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Render skill slot
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GUI::RenderGUI(ShaderHandler * shader_program, PlayerInfoPackage player_data) {
	//Calculate length of health bar based on current health points
	updateHUD(player_data);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(
		GlobalSettings::Access()->ValueOf("WINDOW_WIDTH")),
		0.0f, static_cast<GLfloat>(GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT")));

	shader_program->Use();

	//Projection matrix
	glUniformMatrix4fv(glGetUniformLocation(shader_program->GetProgram(), "in_matrix"), 1, GL_FALSE, glm::value_ptr(projection));

	//Send texture to shaders
	glUniform1i(glGetUniformLocation(shader_program->GetProgram(), "texture_1"), 0);
	glActiveTexture(GL_TEXTURE0);

	//Send marker position to shaders
	glUniform2fv(glGetUniformLocation(shader_program->GetProgram(), "marker_pos"), 1, glm::value_ptr(mm_marker_pos_));


	RenderHealthBar();											// Health bar
	RenderGUIObject(320.0f, 700.0f, 70.0f, current_slot_1_);	// Weapon slot
	RenderGUIObject(400.0f, 700.0f, 70.0f, current_slot_2_);	// Ability slot
	// Render keys in possession
	if (player_data.num_of_keys >= 1) {
		RenderGUIObject(480.0f, 700.0f, 70.0f, key_texture_);
		if (player_data.num_of_keys >= 2) {
			RenderGUIObject(510.0f, 700.0f, 70.0f, key_texture_);
			if (player_data.num_of_keys >= 3) {
				RenderGUIObject(540.0f, 700.0f, 70.0f, key_texture_);
			}
		}
	}
	// Render mini map
	RenderGUIObject(1010.0f, 260.0f, 250.0f, mini_map_);
	// Render mini map player marker
	RenderGUIObject(mm_marker_pos_.x, mm_marker_pos_.y, 10.0f, map_marker_);
}

void GUI::LoadTexture(char * texture_name, GLuint &texture_variable) {
	int tex_width, tex_height;
	unsigned char* image = SOIL_load_image(texture_name, &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);

	std::cout << texture_name << SOIL_last_result() << std::endl;

	glGenTextures(1, &texture_variable);
	glBindTexture(GL_TEXTURE_2D, texture_variable);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}

void GUI::updateHUD(PlayerInfoPackage player_data) {
	//Max length of health bar divided by Max HP, multiplied by current HP
	health_bar_length_ = (300 / player_data.max_hp) * player_data.current_hp;
	//Update map marker position [Coordinates = (PlayerPos / MapResolution) * MiniMapResolution + MiniMapGUIcoords + MarkerOffset / 2]
	mm_marker_pos_ = glm::vec2((player_data.position.x / 2048.0f) * 250.0f + 1010.0f - 5.0f, (player_data.position.y / 2048.0f) * 250.0f + 260 + 7.0f);

	switch (player_data.weapon_id) {
	case WEAPON_NONE:
		current_slot_1_ = quick_slot_texture_;
		break;
	case WEAPON_SWORD:
		current_slot_1_ = quick_slot_sword_texture_;
		break;
	case WEAPON_AXE:
		current_slot_1_ = quick_slot_axe_texture_;
		break;
	};

	switch (player_data.ability_id) {
	case ABILITY_NONE:
		current_slot_2_ = quick_slot_texture_;
		break;
	case ABILITY_DOUBLE_JUMP:
		current_slot_2_ = quick_slot_dbljump_texture_;
		break;
	case ABILITY_DASH:
		current_slot_2_ = quick_slot_dash_texture_;
		break;
	};
}