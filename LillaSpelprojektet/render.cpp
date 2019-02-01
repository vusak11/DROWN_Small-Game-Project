#include "render.h"

void Render::DrawScene() {
}

Render::Render() {
}

Render::~Render() {
}

void Render::InitilizeRender() {
	geometry_pass_ = new ShaderHandler(
		"glsl/geometrypass/geometry_vs.glsl",
		"glsl/geometrypass/geometry_gs.glsl",
		"glsl/geometrypass/geometry_fs.glsl");
}

void Render::UpdateRender(float dt) {
}
