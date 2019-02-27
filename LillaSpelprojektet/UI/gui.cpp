#include "gui.h"

GUI::GUI() {
	health_bar_length_ = (300 / 100) * 100; //Max length of health bar divided by Max HP, multiplied by current HP
}

GUI::~GUI() {

}

void GUI::Initiliaze()
{
	//CONFIG VAO, VBO
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

void GUI::RenderSkillSlots(float xpos, float ypos, float offset) {
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vertex_array_object_);

	GLfloat slot_vertices[4][4] = {
		// Positions					// Texture Coords
		{xpos, ypos,					0.0f, 1.0f},
		{xpos, ypos - offset,			0.0f, 0.0f},
		{xpos + offset, ypos,			1.0f, 1.0f},
		{xpos + offset, ypos - offset,	1.0f, 0.0f},
	};
	//Render texture on quad
	glBindTexture(GL_TEXTURE_2D, quick_slot_texture_);
	
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

	RenderHealthBar();
	RenderSkillSlots(320.0f, 700.0f, 70.0f);
	RenderSkillSlots(400.0f, 700.0f, 70.0f);
}

void GUI::LoadHealthBarTexture(char * texture_name) {
	int tex_width, tex_height;
	unsigned char* image = SOIL_load_image(texture_name, &tex_width, &tex_height, 0, SOIL_LOAD_RGB);

	std::cout << texture_name << SOIL_last_result() << std::endl;

	glGenTextures(1, &health_bar_texture_);
	glBindTexture(GL_TEXTURE_2D, health_bar_texture_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}

void GUI::LoadQuickSlotTexture(char * texture_name) {
	int tex_width, tex_height;
	unsigned char* image = SOIL_load_image(texture_name, &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);

	std::cout << texture_name << SOIL_last_result() << std::endl;

	glGenTextures(1, &quick_slot_texture_);
	glBindTexture(GL_TEXTURE_2D, quick_slot_texture_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}

void GUI::updateHUD(PlayerInfoPackage player_data) {
	//Max length of health bar divided by Max HP, multiplied by current HP
	health_bar_length_ = (300 / player_data.max_hp) * player_data.current_hp;

	/*switch (player_data.ability_1) {
	case 0:
		LoadQuickSlotTexture((char*)"../Resources/GUI/quickslot.png");
	}

	switch (player_data.ability_2) {
	case 0:
		LoadQuickSlotTexture((char*)"../Resources/GUI/quickslot.png");
	}*/
}
