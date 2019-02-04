#include "render.h"

void Render::DrawScene() {
}

Render::Render() {
	quad_vertex_array_object_ = 0;
	quad_vertex_buffer_object_ = 0;

	geometry_pass_ = new ShaderHandler(
		"glsl/geometrypass/geometry_vs.glsl",
		"glsl/geometrypass/geometry_gs.glsl",
		"glsl/geometrypass/geometry_fs.glsl");
	lighting_pass_ = new ShaderHandler(
		"glsl/lightingpass/lighting_vs.glsl",
		"glsl/lightingpass/lighting_fs.glsl");
}

Render::~Render() {
	delete geometry_pass_;
	delete lighting_pass_;
}

void Render::InitializeRender() {
	geometry_pass_->GeometryFrameBuffers();
	lighting_pass_->InitializeLight();
}

void Render::UpdateRender(float dt) {
	geometry_pass_->UpdateGeometry();
	RenderQuad();
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