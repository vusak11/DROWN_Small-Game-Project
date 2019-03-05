#ifndef GUI_H
#define GUI_H

#include "../Globals.h"
#include <iostream>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "../Core/shader_handler.h"

class GUI {
private:
	GLuint health_bar_texture_;
	GLuint key_texture_;
	GLuint quick_slot_texture_;
	GLuint quick_slot_sword_texture_;
	GLuint quick_slot_axe_texture_;
	GLuint quick_slot_dash_texture_;
	GLuint quick_slot_dbljump_texture_;

	GLuint current_slot_1_;
	GLuint current_slot_2_;

	int health_bar_length_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
public:
	GUI();
	~GUI();

	void Initiliaze();

	void RenderHealthBar();
	void RenderGUIObject(float xpos, float ypos, float offset, GLuint slot_texture);

	void RenderGUI(ShaderHandler* shader_program, PlayerInfoPackage player_data);

	void LoadTexture(char* texture_name, GLuint &texture_variable);
	void updateHUD(PlayerInfoPackage player_data);
};

#endif