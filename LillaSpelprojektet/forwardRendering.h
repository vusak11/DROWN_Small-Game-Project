#pragma once
#include <GL/glew.h>
#include "shader_handler.h"
#include "model.h"
#include "map.h"
class ForwardRendering {
private:
	ShaderHandler* forwardRenderingShader;
	
	Model* model_;
	Map* map_;
public:
	ForwardRendering();
	~ForwardRendering();

	unsigned int vertex_buffer_object_, vertex_array_object_;

	void HelloScreen();

	void RenderScreen();

	GLuint GetProgramFromShader() const;

};

