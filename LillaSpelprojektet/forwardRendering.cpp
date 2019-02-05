#include "forwardRendering.h"

ForwardRendering::ForwardRendering() {
	forwardRenderingShader = new ShaderHandler(
		"glsl/forward_vs.glsl",
		"glsl/forward_fs.glsl"
	);
}

ForwardRendering::~ForwardRendering() {
	delete forwardRenderingShader;
}

void ForwardRendering::HelloScreen() {
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f, 
		0.0f, 0.5f, 0.0f
	};	
	glGenVertexArrays(1, &vertex_array_object_);
	glGenBuffers(1, &vertex_buffer_object_);
	glBindVertexArray(vertex_array_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ForwardRendering::RenderScreen() {
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	forwardRenderingShader->Use();
	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLuint ForwardRendering::GetProgramFromShader() const {
	return forwardRenderingShader->GetProgram();
}