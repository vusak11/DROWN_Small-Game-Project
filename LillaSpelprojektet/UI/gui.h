#ifndef GUI_H
#define GUI_H

#include "../global_settings.h"
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
	GLuint quick_slot_texture_;

	int health_bar_length_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
public:
	GUI();
	~GUI();

	void Initiliaze();

	void RenderHealthBar();
	void RenderSkillSlots(float xpos, float ypos, float offset);

	void RenderGUI(ShaderHandler* shader_program, PlayerInfoPackage player_data);

	void LoadHealthBarTexture(char* texture_name);
	void LoadQuickSlotTexture(char* texture_name);
	void updateHUD(PlayerInfoPackage player_data);
};

#endif