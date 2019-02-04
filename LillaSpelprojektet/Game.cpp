#include "game.h"

Game::Game() {

}

Game::~Game() {

}

void Game::InitializeGame() {

	render_.InitializeRender();

}

void Game::DrawScene() {
}

void Game::GameLoop(float dt) {
	event_.InputFromDevices();
	render_.UpdateRender(dt);
}
