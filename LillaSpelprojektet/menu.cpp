#include "menu.h"

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::Initiliaze() {
	//All function return 0 in the case a error occurred
	if (FT_Init_FreeType(&free_type_lib_)) {
		std::cout << "Error::Can't init freetype" << std::endl;
	}
	//Load font
	if (FT_New_Face(
		free_type_lib_,
		"../Resources/Fonts/alittlepot.ttf",
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
			GL_UNSIGNED_INT,
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

void Menu::RenderText(
	ShaderHandler* shader_handler,
	std::string text,
	GLfloat x,
	GLfloat y,
	GLfloat scale,
	glm::vec3 color) {

	shader_handler->Use();
	glUniform3f(
		glGetUniformLocation(shader_handler->GetProgram(), "textColor"), 
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
