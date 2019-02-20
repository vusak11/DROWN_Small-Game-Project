#ifndef RENDER_H
#define RENDER_H

#include "shader_handler.h"
#include "light.h"
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include "../UI/menu.h"
#include "../UI/gui.h"
#include "../Object/Character/model.h"
#include "../Map/map_handler.h"


class Render {
private:
	GLuint quad_vertex_array_object_;
	GLuint quad_vertex_buffer_object_;
	int nr_of_lights_;
	void DrawScene();

	
	ShaderHandler* geometry_pass_;
	ShaderHandler* lighting_pass_;
	Light* lights_;

	int nr_of_models_ = 1;
	Model** model_;

	MapHandler map_handler_;
	//Map map_[1];
	ShaderHandler* text_shaders_;
	ShaderHandler* gui_shaders_;

	glm::mat4 model_matrix_;

	GUI hud_;
public:
	Render();
	~Render();

	void InitializeRender();
	void UpdateRender(
		float dt, 
		glm::vec3 camera_position,
		glm::mat4 perspective_view_matrix,
		std::vector<ObjectPackage>& object_vector,
		PlayerInfoPackage player_data
		);

	void GeometryPass(
		glm::vec3 camera_position,
		glm::mat4 perspective_view_matrix);
	void LightingPass(
		glm::vec3 camera_position);


	void RenderMenuState(Menu menu);
	void RenderPauseMenu(Menu menu);
	void RenderDeathMenu(Menu menu);
	//void GeometryDrawing();
	//void ModelTransformation(glm::vec3 m_translate, glm::vec3 m_rotate, float radians, glm::vec3 m_scale);

	//ON HOLD
	void GeometryDrawing(std::vector<ObjectPackage>& object_vector);
	void ModelTransformation(glm::mat4 model_matrix);
	void RenderQuad();
};
#endif
