#include "game.h"

void Game::DrawScene() {
}

Game::Game() {

}

Game::~Game() {

}

void Game::Initilize() {
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not linking" << std::endl;
	}

	shader_.Initialize("glsl/forward_vs.glsl", "glsl/forward_fs.glsl");
}

void Game::GameLoop(float dt) {
	event_.InputFromDevices();
	render_.UpdateRender(dt);
}
