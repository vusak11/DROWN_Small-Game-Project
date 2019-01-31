#pragma once

#include <GL/glew.h>
//#include <SFML/OpenGL.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class ShaderHandler {
private:
	char info_log_[512];
	int success_;
	GLuint forward_rendering_;

	GLuint geometry_pass_;
	GLuint geometry_buffer_;
	GLuint geometry_position_;
	GLuint geometry_normal_;
	GLuint geometry_albedo_specular_;
public:
	ShaderHandler();
	~ShaderHandler();

	void Initialize(const char* vertex_path, const char* fragment_path);
	void Initialize(const char* vertex_path, const char* geometry_path, const char* fragment_path);
	void CompileStatus(GLuint id, int success, char* info);
	void Use();
};