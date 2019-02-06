#include "forwardRendering.h"

ForwardRendering::ForwardRendering() {
	forwardRenderingShader = new ShaderHandler(
		"glsl/forward_vs.glsl",
		"glsl/forward_gs.glsl",
		"glsl/forward_fs.glsl"
	);

	model_ = new Model((char*)"../Resources/Models/TestBox/testBOX.obj");
	map_ = new Map((char*)"../Resources/Map/TestMap.bmp");
}

ForwardRendering::~ForwardRendering() {
	delete forwardRenderingShader;
	delete model_;
	delete map_;
}

void ForwardRendering::HelloScreen() {
	map_->LoadTexture((char*)"../Resources/Map/rock.png");
	map_->Buffer(forwardRenderingShader->GetProgram());

	glEnable(GL_CULL_FACE);
}

void ForwardRendering::RenderScreen() {
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	forwardRenderingShader->Use();

	model_->Draw(forwardRenderingShader->GetProgram());
}

void ForwardRendering::RenderMap() {
	map_->Draw(forwardRenderingShader->GetProgram());
}

GLuint ForwardRendering::GetProgramFromShader() const {
	return forwardRenderingShader->GetProgram();
}