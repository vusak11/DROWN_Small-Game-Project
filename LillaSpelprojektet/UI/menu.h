#ifndef MENU_H
#define MENU_H

#include "../global_settings.h"
#include <iostream>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Core/shader_handler.h"

class Menu {
private:
	struct Character {
		GLuint texture_id;
		glm::ivec2 size;
		glm::ivec2 bearing;
		GLuint advance;
	};
	std::map<GLchar, Character> characters_;
	
	FT_Library free_type_lib_;
	FT_Face free_type_face_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;

	int selected_item_index_;
	int nr_of_items_;
	

	void RenderText(
		ShaderHandler* shader_handler,
		std::string text,
		GLfloat x,
		GLfloat y,
		GLfloat scale,
		glm::vec3 color
	);
public:

	Menu();
	~Menu();

	void Initiliaze();
	void NavigateUp();
	void NavigateDown();
	void RenderMenu(ShaderHandler* shader_handler);
	void RenderPauseMenu(ShaderHandler* shader_handler);
	void RenderDeathMenu(ShaderHandler* shader_handler);
	void StateManager(GameState state);

	int GetSelectedItemIndex() const;
};

#endif

