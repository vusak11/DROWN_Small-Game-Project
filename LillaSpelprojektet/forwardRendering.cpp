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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ForwardRendering::RenderMenuState(Menu menu_)
{
	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	menu_.RenderMenu(textShader);
}

void ForwardRendering::RenderScreen() {
	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	forwardRenderingShader->Use();

	//map_->Draw(forwardRenderingShader->GetProgram());
	model_->Draw(forwardRenderingShader->GetProgram());
}

GLuint ForwardRendering::GetProgramFromShader() const {
	return forwardRenderingShader->GetProgram();
}