#pragma once
#include "shader_handler.h"
#include "light.h"
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include "model.h"
#include "map.h"


class Render {
private:
	GLuint quad_vertex_array_object_;
	GLuint quad_vertex_buffer_object_;
	int nr_of_lights_;
	void DrawScene();

	ShaderHandler* geometry_pass_;
	ShaderHandler* lighting_pass_;
	Light* lights_;

	int nr_of_models = 1;
	Model** model_;

	Map map_[1];
public:
	Render();
	~Render();

	void InitializeRender();
	void UpdateRender(float dt);

	void GeometryPass();
	void LightingPass();

	void RenderQuad();
};

