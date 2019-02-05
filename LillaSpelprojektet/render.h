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
	unsigned int VBO, VAO;
	glm::mat4 model_matrix_;
public:
	Render();
	~Render();

	void InitializeRender();
	void UpdateRender(
		float dt, 
		glm::vec3 camera_position,
		glm::mat4 perspective_matrix,
		glm::mat4 view_matrix);

	void GeometryPass(
		glm::vec3 camera_position,
		glm::mat4 perspective_matrix,
		glm::mat4 view_matrix);
	void LightingPass(
		glm::vec3 camera_position);

	void RenderQuad();
};

