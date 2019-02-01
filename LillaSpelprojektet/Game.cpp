#include "game.h"

void Game::DrawScene() {
}

Game::Game() {

}

Game::~Game() {

}

void Game::InitializeGame() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not linking" << std::endl;
	}

	render_.InitializeRender();

}

void Game::GameLoop(float dt) {
	event_.InputFromDevices();
	render_.UpdateRender(dt);
}
