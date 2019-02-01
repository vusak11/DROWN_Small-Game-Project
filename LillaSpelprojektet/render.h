#pragma once
#include "shader_handler.h"
#include "light.h"
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

class Render {
private:
	GLuint quadVAO_;
	GLuint quadVBO_;
	void DrawScene();

	ShaderHandler* geometry_pass_;
	ShaderHandler* lighting_pass_;

	Light* lights_;
	int nr_of_lights_;
public:
	Render();
	~Render();

	void InitializeRender();
	void UpdateRender(float dt);

	void GeometryPass();
	void LightingPass();

	void RenderQuad();
};

