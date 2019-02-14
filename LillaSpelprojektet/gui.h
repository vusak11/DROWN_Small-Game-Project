#ifndef GUI_H
#define GUI_H

#include "Globals.h"
#include <iostream>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "shader_handler.h"

class GUI {
private:
	int health_bar_length_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;
public:
	GUI();
	~GUI();
	void RenderGUI(ShaderHandler* shader_program);
}

#endif