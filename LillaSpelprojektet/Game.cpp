#include "game.h"

void Game::DrawScene() {
}

Game::Game() {
	geometry_pass_ = new ShaderHandler(
		"glsl/geometrypass/geometry_vs.glsl", 
		"glsl/geometrypass/geometry_gs.glsl", 
		"glsl/geometrypass/geometry_fs.glsl");
}

Game::~Game() {

}

void Game::InitilizeGame() {
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not linking" << std::endl;
	}

}

void Game::GameLoop(float dt) {
	event_.InputFromDevices();
	render_.UpdateRender(dt);
}
