#include "forwardRendering.h"

ForwardRendering::ForwardRendering() {
	forward_rendering_shaders_ = new ShaderHandler(
		"glsl/forward_vs.glsl",
		"glsl/forward_fs.glsl"
	);
	text_shaders_ = new ShaderHandler(
		"glsl/textshader_vs.glsl",
		"glsl/textshader_fs.glsl"
	);

	model_ = new Model((char*)"../Resources/Models/TestBox/testBOX.obj");
	//map_ = new Map((char*)"../Resources/Map/TestMap.bmp");
}

ForwardRendering::~ForwardRendering() {
	delete forward_rendering_shaders_;
	delete model_;
	//delete map_;
}

void ForwardRendering::HelloScreen() {
	//map_->Buffer(forward_rendering_shaders_->GetProgram());

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ForwardRendering::RenderMenuState(Menu menu_)
{
	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	menu_.RenderMenu(text_shaders_);
}

void ForwardRendering::RenderScreen() {
	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	forward_rendering_shaders_->Use();

	//map_->Draw(forward_rendering_shaders_->GetProgram());
	model_->Draw(forward_rendering_shaders_->GetProgram());
}

GLuint ForwardRendering::GetProgramFromShader() const {
	return forward_rendering_shaders_->GetProgram();
}