#include "menu.h"

Menu::Menu() {
	selected_item_index_ = 0;
	nr_of_items_ = 0;
}

Menu::~Menu() {
}

void Menu::Initialize() {
	mini_map_enabled_ = true;

	//All function return 0 in the case a error occurred
	if (FT_Init_FreeType(&free_type_lib_)) {
		std::cout << "Error::Can't init freetype" << std::endl;
	}
	//Load font
	if (FT_New_Face(
		free_type_lib_,
		"../Resources/Fonts/strangerbackinthenight.ttf",
		0,
		&free_type_face_)) {

		std::cout << "Error::Can't load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(free_type_face_, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Load first 128 characters of ASCII
	for (GLubyte c = 0; c < 128; c++) {
		//Load character glyph
		if (FT_Load_Char(free_type_face_, c, FT_LOAD_RENDER)) {
			std::cout << "Error::Can't load Glyph" << std::endl;
			continue;
		}
		//Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			free_type_face_->glyph->bitmap.width,
			free_type_face_->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			free_type_face_->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Store characters for later use
		Character character = {
			texture,
			glm::ivec2(free_type_face_->glyph->bitmap.width, free_type_face_->glyph->bitmap.rows),
			glm::ivec2(free_type_face_->glyph->bitmap_left, free_type_face_->glyph->bitmap_top),
			free_type_face_->glyph->advance.x
		};
		characters_.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//Destroy Face once finished
	FT_Done_Face(free_type_face_);
	FT_Done_FreeType(free_type_lib_);

	//Config VAO, VBO
	glGenVertexArrays(1, &vertex_array_object_);
	glGenBuffers(1, &vertex_buffer_object_);
	glBindVertexArray(vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * 6 * 4, 
		NULL,
		GL_DYNAMIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		4, 
		GL_FLOAT,
		GL_FALSE,
		4 * sizeof(GLfloat), 
		0
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Menu::NavigateUp() {
	if (selected_item_index_ == 0) {
		selected_item_index_ = (nr_of_items_ - 1);
	}
	else {
		selected_item_index_--;
	}
}

void Menu::NavigateDown()
{
	if (selected_item_index_ == (nr_of_items_ - 1)) {
		selected_item_index_ = 0;
	}
	else {
		selected_item_index_++;
	}
}

void Menu::RenderMenu(ShaderHandler * shader_handler) {
	int window_width = GlobalSettings::Access()->ValueOf("WINDOW_WIDTH");
	int window_height = GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT");

	/*----------------Title---------------*/
	RenderText(
		shader_handler,
		"D R O W N",
		((float)window_width) / 4.0f,
		(((float)window_height) / 10.0f) * 7.0f,
		3.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
	/*----------------End Title-----------*/
	// ---------- START ----------
	if (selected_item_index_ == 0) {
		RenderText(
			shader_handler,
			"Start",
			((float)window_width) / 2.0f - 500.0f,
			(((float)window_height) / 10.0f) * 5.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Start",
			((float)window_width) / 2.0f - 530.0f,
			(((float)window_height) / 10.0f) * 5.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}

	// ---------- OPTIONS ----------
	if (selected_item_index_ == 1) {
		RenderText(
			shader_handler,
			"Options",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 3.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Options",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 3.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}

	// ---------- QUIT ----------
	if (selected_item_index_ == 2) {
		RenderText(
			shader_handler,
			"Quit",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 1.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Quit",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 1.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
}

void Menu::RenderOptionsMenu(ShaderHandler * shader_handler, CameraHandler* cam_handler) {
	int window_width = GlobalSettings::Access()->ValueOf("WINDOW_WIDTH");
	int window_height = GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT");

	/*----------------Title---------------------*/
	RenderText(
		shader_handler,
		"Options",
		((float)window_width) / 9.0f * 3.5f,
		(((float)window_height) / 11.0f) * 9.0f,
		2.5f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
	/*----------------END Title-----------------*/
	//----------------Resolution-------------------*/
	if (selected_item_index_ == 0) {
		RenderText(
			shader_handler,
			"Camera Zoom",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 5.5f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
		RenderText(
			shader_handler,
			"> " + std::to_string((int)std::round(cam_handler->GetCameraPosition().z)) + " <",
			((float)window_width) / 5.0f * 2.0f,
			((float)window_height) / 10.0f * 5.5f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Camera Zoom",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 5.5f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
		RenderText(
			shader_handler,
			std::to_string((int)std::round(cam_handler->GetCameraPosition().z)),
			((float)window_width) / 5.0f * 2.0f,
			((float)window_height) / 10.0f * 5.5f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	//----------------END Resolution---------------*/
	//----------------Volume---------------*/
	if (selected_item_index_ == 1) {
		RenderText(
			shader_handler,
			"Volume",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 4.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
		RenderText(
			shader_handler,
			"> " + std::to_string((int)std::round(sf::Listener::getGlobalVolume())) + " <",
			((float)window_width) / 5.0f * 2.0f,
			((float)window_height) / 10.0f * 4.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Volume",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 4.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
		RenderText(
			shader_handler,
			std::to_string((int)std::round(sf::Listener::getGlobalVolume())),
			((float)window_width) / 5.0f * 2.0f,
			((float)window_height) / 10.0f * 4.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	//----------------END Volume-----------*/
	//----------------XD---------------*/
	if (selected_item_index_ == 2) {
		RenderText(
			shader_handler,
			"Mini Map",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 2.5f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
		if (mini_map_enabled_) {
			RenderText(
				shader_handler,
				"> Enabled <",
				((float)window_width) / 5.0f * 2.0f,
				((float)window_height) / 10.0f * 2.5f,
				2.0f,
				glm::vec3(0.8f, 0.8f, 0.8f)
			);
		}
		else {
			RenderText(
				shader_handler,
				"> Disabled <",
				((float)window_width) / 5.0f * 2.0f,
				((float)window_height) / 10.0f * 2.5f,
				2.0f,
				glm::vec3(0.8f, 0.8f, 0.8f)
			);
		}
	}
	else {
		RenderText(
			shader_handler,
			"Mini Map",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 2.5f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
		if (mini_map_enabled_) {
			RenderText(
				shader_handler,
				"Enabled",
				((float)window_width) / 5.0f * 2.0f,
				((float)window_height) / 10.0f * 2.5f,
				2.0f,
				glm::vec3(0.75f, 0.0f, 0.0f)
			);
		}
		else {
			RenderText(
				shader_handler,
				"Disabled",
				((float)window_width) / 5.0f * 2.0f,
				((float)window_height) / 10.0f * 2.5f,
				2.0f,
				glm::vec3(0.75f, 0.0f, 0.0f)
			);
		}
	}
	//----------------END XD-----------*/
	// ---------- BACK ----------
	if (selected_item_index_ == 3) {
		RenderText(
			shader_handler,
			"Back",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 1.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Back",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 1.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	//----------------END BACK-----------*/
}

void Menu::RenderPauseMenu(ShaderHandler * shader_handler) {
	int window_width = GlobalSettings::Access()->ValueOf("WINDOW_WIDTH");
	int window_height = GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT");

	/*----------------Title---------------------*/
	RenderText(
		shader_handler,
		"Pause",
		((float)window_width) / 9.0f * 3.5f,
		(((float)window_height) / 11.0f) * 8.0f,
		3.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
	/*----------------End Title-----------------*/
	//----------------Continue-------------------*/
	if (selected_item_index_ == 0) {
		RenderText(
			shader_handler,
			"Continue",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 5.5f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Continue",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 5.5f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	//----------------End Continue---------------*/
	//----------------Save score---------------*/
	if (selected_item_index_ == 1) {
		RenderText(
			shader_handler,
			"Save score",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 4.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Save score",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 4.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	//----------------End Save score-----------*/
	//----------------Options---------------*/
	if (selected_item_index_ == 2) {
		RenderText(
			shader_handler,
			"Options",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 2.5f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Options",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 2.5f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	//----------------End Options-----------*/
	// ---------- QUIT ----------
	if (selected_item_index_ == 3) {
		RenderText(
			shader_handler,
			"Quit",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 1.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Quit",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 1.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
}

void Menu::RenderDeathMenu(ShaderHandler * shader_handler) {
	int window_width = GlobalSettings::Access()->ValueOf("WINDOW_WIDTH");
	int window_height = GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT");

	/*----------------Title---------------------*/
	RenderText(
		shader_handler,
		"YOU ARE DEAD",
		((float)window_width) / 5.0f,
		(((float)window_height) / 10.0f) * 7.0f,
		3.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
	/*----------------End Title-----------------*/
	/*----------------Restart-----------------*/
	if (selected_item_index_ == 0) {
		RenderText(
			shader_handler,
			"Restart",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 5.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Restart",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 5.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	/*------------End Restart-----------------*/
	/*----------------Save score-----------------*/
	if (selected_item_index_ == 1) {
		RenderText(
			shader_handler,
			"Save score",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 3.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Save score",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 3.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	/*------------End Save score-----------------*/
	/*------------Quit-----------------*/
	if (selected_item_index_ == 2) {
		RenderText(
			shader_handler,
			"Quit",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 1.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Quit",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 1.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	/*---------End Quit-----------------*/
}

void Menu::RenderVictoryMenu(ShaderHandler * shader_handler) {
	int window_width = GlobalSettings::Access()->ValueOf("WINDOW_WIDTH");
	int window_height = GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT");

	/*----------------Title---------------------*/
	RenderText(
		shader_handler,
		"YOU WON",
		((float)window_width) / 5.0f,
		(((float)window_height) / 10.0f) * 7.0f,
		3.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
	/*----------------End Title-----------------*/
	/*----------------Restart-----------------*/
	if (selected_item_index_ == 0) {
		RenderText(
			shader_handler,
			"Restart",
			((float)window_width) / 2.0f - 500.0f,
			((float)window_height) / 10.0f * 5.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Restart",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 5.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}
	if (selected_item_index_ == 1) {
		RenderText(
			shader_handler,
			"Quit",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 3.0f,
			2.0f,
			glm::vec3(0.8f, 0.8f, 0.8f)
		);
	}
	else {
		RenderText(
			shader_handler,
			"Quit",
			((float)window_width) / 2.0f - 530.0f,
			((float)window_height) / 10.0f * 3.0f,
			2.0f,
			glm::vec3(0.75f, 0.0f, 0.0f)
		);
	}

}

void Menu::StateManager(GameState state) {
	if (state == MENU || state == DEATH) {
		nr_of_items_ = 3;
	}
	else if (state == PAUSE) {
		nr_of_items_ = 4;
	}
	else if (state == OPTIONS) {
		nr_of_items_ = 4;
	}
	else if (state == VICTORY) {
		nr_of_items_ = 2;
	}
}

int Menu::GetSelectedItemIndex() const {
	return selected_item_index_;
}

void Menu::SetSelectedItemIndex(int index) {
	selected_item_index_ = index;
}

void Menu::RenderText(
	ShaderHandler* shader_handler,
	std::string text,
	GLfloat x,
	GLfloat y,
	GLfloat scale,
	glm::vec3 color) {

	int window_width = GlobalSettings::Access()->ValueOf("WINDOW_WIDTH");
	int window_height = GlobalSettings::Access()->ValueOf("WINDOW_HEIGHT");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(window_width),
		0.0f, static_cast<GLfloat>(window_height));

	shader_handler->Use();
	glUniformMatrix4fv(glGetUniformLocation(shader_handler->GetProgram(), "in_matrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(
		glGetUniformLocation(shader_handler->GetProgram(), "text_color"), 
		color.x,
		color.y,
		color.z
	);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vertex_array_object_);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters_[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Menu::IsMinIMapEnabled() {
	return mini_map_enabled_;
}

void Menu::SetMiniMap(bool enabled) {
	mini_map_enabled_ = enabled;
}