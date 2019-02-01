#pragma once
#include "shader_handler.h"

class Render {
private:
	GLuint quadVAO_;
	GLuint quadVBO_;
	void DrawScene();
	ShaderHandler* geometry_pass_;
public:
	Render();
	~Render();

	void InitializeRender();
	void UpdateRender(float dt);
	void RenderQuad();
};

