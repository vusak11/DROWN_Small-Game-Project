#include "shader_handler.h"



ShaderHandler::ShaderHandler() {
	this->success_ = 0;

	this->forward_rendering_ = 0;

	this->geometry_pass_ = 0;
	this->geometry_buffer_ = 0;
	this->geometry_position_ = 0;
	this->geometry_normal_ = 0;
	this->geometry_albedo_specular_ = 0;
}

ShaderHandler::~ShaderHandler() {

}

void ShaderHandler::Initialize(const char* vertex_path, const char* geometry_path, const char* fragment_path) {
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
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &this->success_);
	if (!this->success_) {
		glGetShaderInfoLog(vertex, 512, NULL, this->info_log_);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << this->info_log_ << std::endl;
	}
	else {
		std::cout << "VERTEX::COMPILATION_WORKED " << this->success_ << std::endl;
	}

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
	glGetShaderiv(geometry, GL_COMPILE_STATUS, &this->success_);
	if (!this->success_) {
		glGetShaderInfoLog(geometry, 512, NULL, this->info_log_);
		std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << this->info_log_ << std::endl;
	}
	else {
		std::cout << "GEOMETRY::COMPILATION_WORKED " << this->success_ << std::endl;
	}
	

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
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &this->success_);
	if (!this->success_) {
		glGetShaderInfoLog(fragment, 512, NULL, this->info_log_);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << this->info_log_ << std::endl;
	}
	else {
		std::cout << "FRAGMENT::COMPILATION_WORKED " << this->success_ << std::endl;
	}

	//Shader program
	this->forward_rendering_ = glCreateProgram();
	glAttachShader(this->forward_rendering_, vertex);
	glAttachShader(this->forward_rendering_, geometry);
	glAttachShader(this->forward_rendering_, fragment);
	glLinkProgram(this->forward_rendering_);

	glGetProgramiv(this->forward_rendering_, GL_LINK_STATUS, &success_);
	if (!success_) {
		glGetProgramInfoLog(this->forward_rendering_, 512, NULL, this->info_log_);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << this->info_log_ << std::endl;
	}
}

void ShaderHandler::CompileStatus(GLuint id, int success, char* info) {
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, info);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info << std::endl;
	}
}

void ShaderHandler::Use() {
	glUseProgram(this->forward_rendering_);
}