#include "shader_handler.h"

void ShaderHandler::GeometryPass() {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, buffer_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Use();

	glUniform3fv(glGetUniformLocation(
		program_, "view_position"), 1, glm::value_ptr(glm::vec3(2, 2, 2))); //OBS camera position
	glUniformMatrix4fv(glGetUniformLocation(
		program_, "projection"), 1, GL_FALSE, glm::value_ptr(glm::vec3(2, 2, 2))); //OBS projection
	glUniformMatrix4fv(glGetUniformLocation(
		program_, "view"), 1, GL_FALSE, glm::value_ptr(glm::vec3(2, 2, 2))); //OBS view

}

void ShaderHandler::LightingPass() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Use();

	glUniform1i(glGetUniformLocation(program_, "g_position"), 0);
	glUniform1i(glGetUniformLocation(program_, "g_normal"), 1);
	glUniform1i(glGetUniformLocation(program_, "g_albedo_spec"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, position_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, albedo_specular_);

	std::string name;
	for (int i = 0; i < nr_of_lights_; i++) {
		name = "lights[" + std::to_string(i) + "].position_";
		glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, glm::value_ptr(lights_[i].GetPos()));
		name = "lights[" + std::to_string(i) + "].color_";
		glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, glm::value_ptr(lights_[i].GetAmbientLight()));
	}

	glUniform3fv(glGetUniformLocation(program_->GetProgram(), "viewPos"), 1, glm::value_ptr(glm::vec3(2, 2, 2))); //Add camera positions

}

ShaderHandler::ShaderHandler() {
	this->success_ = 0;

	this->program_ = 0;

	this->buffer_ = 0;
	this->position_ = 0;
	this->normal_ = 0;
	this->albedo_specular_ = 0;

	nr_of_lights_ = 1;
	lights_ = new Light[nr_of_lights_];
}

ShaderHandler::ShaderHandler(
	const char* vertex_path,
	const char* fragment_path) {

	//  Variables
	this->success_ = 0;

	this->program_ = 0;
	this->buffer_ = 0;
	this->position_ = 0;
	this->normal_ = 0;
	this->albedo_specular_ = 0;
	nr_of_lights_ = 1;
	lights_ = new Light[nr_of_lights_];

	//				Vertex shader
	//Open and retrieve VERTEX file content
	std::ifstream glsl_file(vertex_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << 
			vertex_path << std::endl;
	}
	std::string glsl_text((
		std::istreambuf_iterator<char>(glsl_file)), 
		std::istreambuf_iterator<char>()
	);
	glsl_file.close();
	const char* glsl_data = glsl_text.c_str();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &glsl_data, NULL);
	glCompileShader(vertex);

	//Shader check
	CompileStatus(vertex, this->success_, this->info_log_, SHADER);

	//				Fragment shader
	//Open and retrieve FRAGMENT file content
	glsl_file.open(fragment_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" <<
			fragment_path << std::endl;
	}
	glsl_text.assign((
		std::istreambuf_iterator<char>(glsl_file)),
		std::istreambuf_iterator<char>());
	glsl_file.close();
	glsl_data = glsl_text.c_str();

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &glsl_data, NULL);
	glCompileShader(fragment);

	//Shader check
	CompileStatus(fragment, this->success_, this->info_log_, SHADER);

	this->program_ = glCreateProgram();
	glAttachShader(this->program_, vertex);
	glAttachShader(this->program_, fragment);
	glLinkProgram(this->program_);

	CompileStatus(this->program_, this->success_, this->info_log_, PROGRAM);
}

ShaderHandler::ShaderHandler(
	const char* vertex_path, 
	const char* geometry_path, 
	const char* fragment_path) {

	//  Variables
	this->success_ = 0;

	this->program_ = 0;
	this->buffer_ = 0;
	this->position_ = 0;
	this->normal_ = 0;
	this->albedo_specular_ = 0;
	nr_of_lights_ = 1;
	lights_ = new Light[nr_of_lights_];
	
	//				Vertex shader
	//Open and retrieve VERTEX file content
	std::ifstream glsl_file(vertex_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << 
			vertex_path << std::endl;
	}
	std::string glsl_text((
		std::istreambuf_iterator<char>(glsl_file)),
		std::istreambuf_iterator<char>()
	);
	glsl_file.close();
	const char* glsl_data = glsl_text.c_str();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &glsl_data, NULL);
	glCompileShader(vertex);

	//Error handler
	CompileStatus(vertex, this->success_, this->info_log_, SHADER);

	//				Geometry shader
	//Open and retrieve GEOMETRY file content
	glsl_file.open(geometry_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OENED\n" << 
			geometry_path << std::endl;
	}

	glsl_text.assign((
		std::istreambuf_iterator<char>(glsl_file)), 
		std::istreambuf_iterator<char>()
	);
	glsl_file.close();
	glsl_data = glsl_text.c_str();

	GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &glsl_data, NULL);
	glCompileShader(geometry);

	//Error handler
	CompileStatus(geometry, this->success_, this->info_log_, SHADER);

	//				Fragment shader
	//Open and retrieve FRAGMENT file content
	glsl_file.open(fragment_path);
	if (!(glsl_file.is_open())) {
		std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << 
			fragment_path << std::endl;
	}
	glsl_text.assign((
		std::istreambuf_iterator<char>(glsl_file)), 
		std::istreambuf_iterator<char>()
	);
	glsl_file.close();
	glsl_data = glsl_text.c_str();

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &glsl_data, NULL);
	glCompileShader(fragment);

	//Error handler
	CompileStatus(fragment, this->success_, this->info_log_, SHADER);

	//Shader program
	this->program_ = glCreateProgram();
	glAttachShader(this->program_, vertex);
	glAttachShader(this->program_, geometry);
	glAttachShader(this->program_, fragment);
	glLinkProgram(this->program_);

	CompileStatus(this->program_, this->success_, this->info_log_, PROGRAM);
}

ShaderHandler::~ShaderHandler() {

}

void ShaderHandler::CompileStatus(GLuint gl_id, int success, char* info, GL_ID gl_type) {
	if (gl_type == SHADER) {
		glGetShaderiv(gl_id, GL_COMPILE_STATUS, &success);
		GLint shader_type;
		glGetShaderiv(gl_id, GL_SHADER_TYPE, &shader_type);
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
			glGetShaderInfoLog(gl_id, 512, NULL, info);
			std::cout << "ERROR::SHADER::" << shader_name << "::COMPILATION_FAILED\n" <<
				info << std::endl;
		}
	}
	else if (gl_type == PROGRAM) {
		glGetProgramiv(gl_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(gl_id, 512, NULL, info);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
				info << std::endl;
		}
	}
}

void ShaderHandler::GeometryFrameBuffers() {
	//Generate Framebuffer
	glGenFramebuffers(1, &this->buffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, this->buffer_);

	//  Color buffer - Position
	glGenTextures(1, &this->position_);
	glBindTexture(GL_TEXTURE_2D, this->position_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 
		0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		this->position_, 0);

	// Color buffer - Normal
	glGenTextures(1, &normal_);
	glBindTexture(GL_TEXTURE_2D, this->normal_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT,
		0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
		this->normal_, 0);

	// Color buffer - Albedo Specular
	glGenTextures(1, &this->albedo_specular_);
	glBindTexture(GL_TEXTURE_2D, this->albedo_specular_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
		this->albedo_specular_, 0);

	// Attachment of the framebuffer we will render
	GLuint attachments[3] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2 
	};
}

GLuint ShaderHandler::GetProgram() {
	return this->program_;
}

GLuint ShaderHandler::GetBuffer() {
	return this->buffer_;
}

GLuint ShaderHandler::GetPosition() {
	return this->position_;
}

GLuint ShaderHandler::GetNormal() {
	return this->normal_;
}

GLuint ShaderHandler::GetAlbedoSpecular() {
	return this->albedo_specular_;
}

void ShaderHandler::Use() {
	glUseProgram(this->program_);
}

void ShaderHandler::UpdateGeometry() {
	GeometryPass();
	LightingPass();
}

void ShaderHandler::InitializeLight() {
	lights_[0].LightDefault(
		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));
}

