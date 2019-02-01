#ifndef SHADERHANLDER_H
#define SHADERHANDLER_H

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

	GLuint GetProgram() const;

	void Initialize(const char* vertex_path, const char* fragment_path);
	void Use();

	GLuint GetProgram() const;
};

#endif