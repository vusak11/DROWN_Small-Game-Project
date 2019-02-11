#include "forwardRendering.h"

ForwardRendering::ForwardRendering() {
	forwardRenderingShader = new ShaderHandler(
		"glsl/forward_vs.glsl",
		"glsl/forward_fs.glsl"
	);
	textShader = new ShaderHandler(
		"glsl/textshader_vs.glsl",
		"glsl/textshader_fs.glsl"
	);

	model_ = new Model((char*)"../Resources/Models/TestBox/testBOX.obj");
	//map_ = new Map((char*)"../Resources/Map/TestMap.bmp");
}

ForwardRendering::~ForwardRendering() {
	delete forwardRenderingShader;
	delete model_;
	//delete map_;
}

void ForwardRendering::HelloScreen() {
	//map_->Buffer(forwardRenderingShader->GetProgram());

	glEnable(GL_CULL_FACE);
	menu_.Initiliaze();
}

void ForwardRendering::RenderScreen() {
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	//forwardRenderingShader->Use();

	//map_->Draw(forwardRenderingShader->GetProgram());
	//model_->Draw(forwardRenderingShader->GetProgram());


	menu_.RenderText(
		textShader,
		"POOPsicle",
		((float)WINDOW_WIDTH)/2.0f,
		((float)WINDOW_HEIGHT)/2.0f,
		2.0f,
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
}

GLuint ForwardRendering::GetProgramFromShader() const {
	return forwardRenderingShader->GetProgram();
}