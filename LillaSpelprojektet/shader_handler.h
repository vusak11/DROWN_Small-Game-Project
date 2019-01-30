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
public:
	ShaderHandler();
	~ShaderHandler();

	void Initialize(const char* vertex_path, const char* fragment_path);
	void Use();
};