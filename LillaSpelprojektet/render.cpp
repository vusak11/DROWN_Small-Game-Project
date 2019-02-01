#include "render.h"

void Render::DrawScene() {
}

Render::Render() {
	quadVAO_ = 0;
	quadVBO_ = 0;
	nr_of_lights_ = 0;
}

Render::~Render() {
}

void Render::InitializeRender() {
	geometry_pass_ = new ShaderHandler(
		"glsl/geometrypass/geometry_vs.glsl",
		"glsl/geometrypass/geometry_gs.glsl",
		"glsl/geometrypass/geometry_fs.glsl");
	lighting_pass_ = new ShaderHandler(
		"glsl/lightingpass/lighting_vs.glsl",
		"glsl/lightingpass/lighting_fs.glsl");


	nr_of_lights_ = 1;
	lights_ = new Light[nr_of_lights_];
	lights_[0].LightDefault(
		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

}

void Render::UpdateRender(float dt) {
	GeometryPass();
	LightingPass();
}

void Render::GeometryPass() {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, geometry_pass_->GetBuffer());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	geometry_pass_->Use();

	glUniform3fv(glGetUniformLocation(
		geometry_pass_->GetProgram(), "view_position"), 1, glm::value_ptr(glm::vec3(2, 2, 2))); //OBS camera position
	glUniformMatrix4fv(glGetUniformLocation(
		geometry_pass_->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(glm::vec3(2, 2, 2))); //OBS projection
	glUniformMatrix4fv(glGetUniformLocation(
		geometry_pass_->GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(glm::vec3(2, 2, 2))); //OBS view

	//Render objects
}

void Render::LightingPass() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lighting_pass_->Use();

	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_position"), 0);
	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_normal"), 1);
	glUniform1i(glGetUniformLocation(lighting_pass_->GetProgram(), "g_albedo_spec"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lighting_pass_->GetPosition());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lighting_pass_->GetNormal());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, lighting_pass_->GetAlbedoSpecular());

	std::string name;
	for (int i = 0; i < nr_of_lights_; i++) {
		name = "lights[" + std::to_string(i) + "].position_";
		glUniform3fv(glGetUniformLocation(lighting_pass_->GetProgram(), name.c_str()), 1, glm::value_ptr(lights_[i].GetPos()));
		name = "lights[" + std::to_string(i) + "].color_";
		glUniform3fv(glGetUniformLocation(lighting_pass_->GetProgram(), name.c_str()), 1, glm::value_ptr(lights_[i].GetAmbientLight()));
	}
	
	glUniform3fv(glGetUniformLocation(lighting_pass_->GetProgram(), "viewPos"), 1, glm::value_ptr(glm::vec3(2, 2, 2))); //Add camera positions

	RenderQuad();
}

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
void Render::RenderQuad() {
	if (quadVAO_ == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO_);
		glGenBuffers(1, &quadVBO_);
		glBindVertexArray(quadVAO_);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}