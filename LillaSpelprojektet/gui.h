#ifndef GUI_H
#define GUI_H

#include "Globals.h"
#include <iostream>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "shader_handler.h"

class GUI {
private:
	GLuint health_bar_texture_;
	GLuint quick_slot_texture_;

	int health_bar_length_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
public:
	GUI();
	~GUI();
	void RenderHealthBar();
	void RenderSkillSlots();
	void RenderGUI(ShaderHandler* shader_program);
	void LoadHealthBarTexture(char* texture_name);
	void LoadQuickSlotTexture(char* texture_name);
};

#endif