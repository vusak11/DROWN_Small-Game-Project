#include "gui.h"

GUI::GUI() {
	health_bar_length_ = (300 / 100) * 100; //Max length of health bar divided by Max HP, multiplied by current HP
}

GUI::~GUI() {

}

void GUI::RenderGUI(ShaderHandler* shader_program) {
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WINDOW_WIDTH), 0.0f, static_cast<GLfloat>(WINDOW_HEIGHT));
	
	shader_program->Use();
	//Projection matrix
	glUniformMatrix4fv(glGetUniformLocation(shader_program->GetProgram(), "in_matrix"), 1, GL_FALSE, glm::value_ptr(projection));
	//Health bar color
	glUniform3f(glGetUniformLocation(shader_program->GetProgram(), "color"), 0.40f, 0.83f, 0.07f);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vertex_array_object_);

	GLfloat bar_vertices[] = {
		// Positions		// Texture Coords
		-0.8f,  0.8f, 0.0f, 0.0f, 1.0f,
		-0.8f,  0.6f, 0.0f, 0.0f, 0.0f,
		-0.2f,  0.8f, 0.0f, 1.0f, 1.0f,
		-0.2f,  0.6f, 0.0f, 1.0f, 0.0f,
	};
	//glBindTexture(GL_TEXTURE_2D, textureID);

	//CONFIG VAO, VBO
	glGenVertexArrays(1, &vertex_array_object_);
	glGenBuffers(1, &vertex_buffer_object_);
	glBindVertexArray(vertex_array_object_);
	// Update content of VBO memory
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bar_vertices), &bar_vertices, GL_STATIC_DRAW); // Be sure to use glBufferSubData and not glBufferData

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Render quad
	glDrawArrays(GL_TRIANGLES, 0, 4);

	glBindVertexArray(0);
}
