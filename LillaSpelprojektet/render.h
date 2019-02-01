#pragma once

#include "GL/glew.h"

class Render {
private:
	GLuint quadVAO_;
	GLuint quadVBO_;
	void DrawScene();
public:
	Render();
	~Render();

	void InitializeRender();
	void UpdateRender(float dt);
	void RenderQuad();
};

