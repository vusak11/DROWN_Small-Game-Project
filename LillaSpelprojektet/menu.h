#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader_handler.h"

class Menu {
private:

	struct Character {
		GLuint texture_id;
		glm::ivec2 size;
		glm::ivec2 bearing;
		GLuint advance;
	};


	FT_Library free_type_lib_;
	FT_Face free_type_face_;

	std::map<GLchar, Character> characters_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
public:
	Menu();
	~Menu();

	void Initiliaze();
	void RenderText(
		ShaderHandler* shader_handler,
		std::string text,
		GLfloat x,
		GLfloat y, 
		GLfloat scale,
		glm::vec3 color
	);
};

#endif

