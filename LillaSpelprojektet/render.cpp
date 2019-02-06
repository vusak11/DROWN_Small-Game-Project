#include "render.h"

void Render::DrawScene() {
}

Render::Render() {
	quad_vertex_array_object_ = 0;
	quad_vertex_buffer_object_ = 0;
	nr_of_lights_ = 1;

	geometry_pass_ = new ShaderHandler(
		"glsl/geometrypass/geometry_vs.glsl",
		"glsl/geometrypass/geometry_gs.glsl",
		"glsl/geometrypass/geometry_fs.glsl");
	lighting_pass_ = new ShaderHandler(
		"glsl/lightingpass/lighting_vs.glsl",
		"glsl/lightingpass/lighting_fs.glsl");
	lights_ = new Light[nr_of_lights_];

	//model_ = new Model*[nr_of_models];
	//model_[0] = new Model((char*)"../Resources/Models/TestCharacter.obj");

	map_[0].LoadMap((char*)"../Resources/Map/TestMap.bmp");
	map_[0].LoadTexture((char*)"../Resources/Map/rock.png");
}

Render::~Render() {
	delete geometry_pass_;
	delete lighting_pass_;
	delete[] lights_;

	/*for (int i = 0; i < nr_of_models; i++) {
		delete model_[i];
	}
	delete[] model_;*/
}

void Render::InitializeRender() {
	geometry_pass_->GeometryFrameBuffers();

	lights_[0].LightDefault(
		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));
	
	float vertices[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	map_[0].Buffer(geometry_pass_->GetProgram());
}

void Render::UpdateRender(
	float dt, 
	glm::vec3 camera_position,
	glm::mat4 perspective_matrix, 
	glm::mat4 view_matrix) {
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	GeometryPass(camera_position, perspective_matrix, view_matrix);

	GeometryDrawing();
	
	lights_[0].SetPos(camera_position);

	LightingPass(camera_position);

	RenderQuad();
}

void Render::GeometryDrawing() {
	ModelTransformation(
		glm::vec3(-256.0f, 0.0, 0.0f),
		glm::vec3(1, 1, 1),
		0,
		glm::vec3(0.8f, 0.8f, 0.8f)
	);
	map_[0].Draw(geometry_pass_->GetProgram());
}

void Render::ModelTransformation(glm::vec3 m_translate, glm::vec3 m_rotate, float radians, glm::vec3 m_scale) {
	model_matrix_ = glm::mat4(1.0f);
	model_matrix_ = glm::translate(model_matrix_, m_translate);
	model_matrix_ = glm::rotate(model_matrix_, glm::radians(radians), m_rotate);
	model_matrix_ = glm::scale(model_matrix_, m_scale);
	glUniformMatrix4fv(glGetUniformLocation(geometry_pass_->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model_matrix_));
}

void Render::GeometryPass(
	glm::vec3 camera_position,
	glm::mat4 perspective_matrix,
	glm::mat4 view_matrix) {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, geometry_pass_->GetBuffer());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	geometry_pass_->Use();

	glUniform3fv(glGetUniformLocation(
		geometry_pass_->GetProgram(), "view_position"), 1, glm::value_ptr(camera_position)); //OBS camera position
	glUniformMatrix4fv(glGetUniformLocation(
		geometry_pass_->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(perspective_matrix)); //OBS projection
	glUniformMatrix4fv(glGetUniformLocation(
		geometry_pass_->GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view_matrix)); //OBS view
}

void Render::LightingPass(glm::vec3 camera_position) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lighting_pass_->Use();

	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_position"), 0);
	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_normal"), 1);
	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_albedo_spec"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, geometry_pass_->GetPosition());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, geometry_pass_->GetNormal());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, geometry_pass_->GetAlbedoSpecular());

	std::string name;
	/*for (int i = 0; i < nr_of_lights_; i++) {
		name = "lights[" + std::to_string(i) + "].position";
		glUniform3fv(glGetUniformLocation(lighting_pass_->GetProgram(), name.c_str()), 1, glm::value_ptr(lights_[i].GetPos()));
		name = "lights[" + std::to_string(i) + "].color";
		glUniform3fv(glGetUniformLocation(lighting_pass_->GetProgram(), name.c_str()), 1, glm::value_ptr(lights_[i].GetAmbientLight()));
	}
	*/
	glUniform3f(
		glGetUniformLocation(lighting_pass_->GetProgram(), "light_position"),
		camera_position.x, camera_position.y, camera_position.z
	);
	glUniform3fv(glGetUniformLocation(lighting_pass_->GetProgram(), "viewPos"), 1, glm::value_ptr(camera_position)); //Add camera positions

}

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
void Render::RenderQuad() {
	if (quad_vertex_array_object_ == 0)
	{
		GLfloat quad_vertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quad_vertex_array_object_);
		glGenBuffers(1, &quad_vertex_buffer_object_);
		glBindVertexArray(quad_vertex_array_object_);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_buffer_object_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quad_vertex_array_object_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}