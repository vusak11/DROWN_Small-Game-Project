#pragma once
#include "Globals.h"
#include <GL/glew.h>
#include "shader_handler.h"
#include "model.h"
#include "map.h"
#include "menu.h"
class ForwardRendering {
private:
	ShaderHandler* forward_rendering_shaders_;
	ShaderHandler* text_shaders_;
	
	Model* model_;
	Map* map_;
public:
	ForwardRendering();
	~ForwardRendering();

	unsigned int vertex_buffer_object_, vertex_array_object_;

	void HelloScreen();

	void RenderMenuState(Menu menu_);
	void RenderScreen();

	GLuint GetProgramFromShader() const;
};

