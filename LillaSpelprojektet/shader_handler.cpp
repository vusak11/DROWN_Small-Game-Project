#include "shader_handler.h"



ShaderHandler::ShaderHandler() {
	this->success_ = 0;

	this->program_ = 0;

	this->geometry_buffer_ = 0;
	this->geometry_position_ = 0;
	this->geometry_normal_ = 0;
	this->geometry_albedo_specular_ = 0;
}

ShaderHandler::ShaderHandler(const char* vertex_path, const char* fragment_path) {
	//  Variables
	this->success_ = 0;
	this->program_ = 0;

	//				Vertex shader
	//Open and retrieve VERTEX file content
	std::ifstream glsl_file(vertex_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << vertex_path << std::endl;
	}
	std::string glsl_text((std::istreambuf_iterator<char>(glsl_file)), std::istreambuf_iterator<char>());
	glsl_file.close();
	const char* glsl_data = glsl_text.c_str();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &glsl_data, NULL);
	glCompileShader(vertex);

	//Shader check
	CompileStatus(vertex, this->success_, this->info_log_);

	//				Fragment shader
	//Open and retrieve FRAGMENT file content
	glsl_file.open(fragment_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << fragment_path << std::endl;
	}
	glsl_text.assign((std::istreambuf_iterator<char>(glsl_file)), std::istreambuf_iterator<char>());
	glsl_file.close();
	glsl_data = glsl_text.c_str();

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &glsl_data, NULL);
	glCompileShader(fragment);

	//Shader check
	CompileStatus(fragment, this->success_, this->info_log_);

	this->program_ = glCreateProgram();
	glAttachShader(this->program_, vertex);
	glAttachShader(this->program_, fragment);
	glLinkProgram(this->program_);

	glGetProgramiv(this->program_, GL_LINK_STATUS, &success_);
	if (!success_) {
		glGetProgramInfoLog(this->program_, 512, NULL, this->info_log_);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << this->info_log_ << std::endl;
	}
	else {
		std::cout << "SHADER::PROGRAM::LINKED\n" << std::endl;
	}
}

ShaderHandler::ShaderHandler(const char* vertex_path, const char* geometry_path, const char* fragment_path) {
	//  Variables
	this->success_ = 0;
	this->program_ = 0;
	
	//				Vertex shader
	//Open and retrieve VERTEX file content
	std::ifstream glsl_file(vertex_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << vertex_path << std::endl;
	}
	std::string glsl_text((std::istreambuf_iterator<char>(glsl_file)), std::istreambuf_iterator<char>());
	glsl_file.close();
	const char* glsl_data = glsl_text.c_str();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &glsl_data, NULL);
	glCompileShader(vertex);

	//Error handler
	CompileStatus(vertex, this->success_, this->info_log_);

	//				Geometry shader
	//Open and retrieve GEOMETRY file content
	glsl_file.open(geometry_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OENED\n" << geometry_path << std::endl;
	}

	glsl_text.assign((std::istreambuf_iterator<char>(glsl_file)), std::istreambuf_iterator<char>());
	glsl_file.close();
	glsl_data = glsl_text.c_str();

	GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &glsl_data, NULL);
	glCompileShader(geometry);

	//Error handler
	CompileStatus(vertex, this->success_, this->info_log_);

	//				Fragment shader
	//Open and retrieve FRAGMENT file content
	glsl_file.open(fragment_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << fragment_path << std::endl;
	}
	glsl_text.assign((std::istreambuf_iterator<char>(glsl_file)), std::istreambuf_iterator<char>());
	glsl_file.close();
	glsl_data = glsl_text.c_str();

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &glsl_data, NULL);
	glCompileShader(fragment);

	//Error handler
	CompileStatus(vertex, this->success_, this->info_log_);

	//Shader program
	this->program_ = glCreateProgram();
	glAttachShader(this->program_, vertex);
	glAttachShader(this->program_, geometry);
	glAttachShader(this->program_, fragment);
	glLinkProgram(this->program_);

	glGetProgramiv(this->program_, GL_LINK_STATUS, &success_);
	if (!success_) {
		glGetProgramInfoLog(this->program_, 512, NULL, this->info_log_);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << this->info_log_ << std::endl;
	}
	else {
		std::cout << "SHADER::PROGRAM::LINKED\n" << std::endl;
	}
}

ShaderHandler::~ShaderHandler() {

}

void ShaderHandler::CompileStatus(GLuint shader_id, int success, char* info) {
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	GLint shader_type;
	glGetShaderiv(shader_id, GL_SHADER_TYPE, &shader_type);
	std::string shader_name = "";
	switch (shader_type) {
	case GL_VERTEX_SHADER:
		shader_name = "VERTEX";
		break;
	case GL_GEOMETRY_SHADER:
		shader_name = "GEOMETRY";
		break;
	case GL_FRAGMENT_SHADER:
		shader_name = "FRAGMENT";
		break;
	}
	if (!success) {
		glGetShaderInfoLog(shader_id, 512, NULL, info);
		std::cout << "ERROR::SHADER::" << shader_name << "::COMPILATION_FAILED\n" << info << std::endl;
	}
	else {
		std::cout << shader_name << "::COMPILATION_WORKED\n" << std::endl;
	}
}

void ShaderHandler::GeometryPass() {
	glGenFramebuffers(1, &this->geometry_buffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, this->geometry_buffer_);

	//  Color buffer - Position
	glGenTextures(1, &this->geometry_position_);
	glBindTexture(GL_TEXTURE_2D, this->geometry_position_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->geometry_position_, 0);

	// Color buffer - Normal
	glGenTextures(1, &geometry_normal_);
	glBindTexture(GL_TEXTURE_2D, this->geometry_normal_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->geometry_normal_, 0);

	// Color buffer - Albedo Specular
	glGenTextures(1, &this->geometry_albedo_specular_);
	glBindTexture(GL_TEXTURE_2D, this->geometry_albedo_specular_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->geometry_albedo_specular_, 0);
}

void ShaderHandler::Use() {
	glUseProgram(this->program_);
}

