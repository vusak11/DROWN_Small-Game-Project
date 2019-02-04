#ifndef SHADER_HANDLER_H
#define SHADER_HANDLER_H
#endif
#include "Globals.h"
#include <GL/glew.h>
//#include <SFML/OpenGL.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class ShaderHandler {
private:
	enum GL_ID {
		SHADER,
		PROGRAM
	};

	char info_log_[512];
	int success_;

	GLuint program_;

	//  Geometry
	GLuint buffer_;
	GLuint position_;
	GLuint normal_;
	GLuint albedo_specular_;
public:
	ShaderHandler();
	ShaderHandler(const char* vertex_path, const char* fragment_path);
	ShaderHandler(const char* vertex_path, const char* geometry_path, const char* fragment_path);
	~ShaderHandler();

	void CompileStatus(GLuint shader_id, int success, char* info, GL_ID gl_type);
	
	GLuint GetProgram();
	GLuint GetBuffer();
	GLuint GetPosition();
	GLuint GetNormal();
	GLuint GetAlbedoSpecular();

	void GeometryPass();

	void Use();

};