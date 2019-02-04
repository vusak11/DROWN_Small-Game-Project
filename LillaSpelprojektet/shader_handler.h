#ifndef SHADER_HANDLER_H
#define SHADER_HANDLER_H
#include "Globals.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "light.h"

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

	Light* lights_;
	int nr_of_lights_;

	void GeometryPass();
	void LightingPass();

public:
	ShaderHandler();
	ShaderHandler(const char* vertex_path, const char* fragment_path);
	ShaderHandler(const char* vertex_path, const char* geometry_path, const char* fragment_path);
	~ShaderHandler();

	GLuint GetProgram();
	GLuint GetBuffer();
	GLuint GetPosition();
	GLuint GetNormal();
	GLuint GetAlbedoSpecular();

	void GeometryFrameBuffers();
	void CompileStatus(GLuint shader_id, int success, char* info, GL_ID gl_type);
	void Use();
	void UpdateGeometry();
	void InitializeLight();

};
#endif