#include "game.h"

void Game::DrawScene() {
}

Game::Game() {

}

Game::~Game() {

}

void Game::InitilizeGame() {
	render_.InitilizeRender();
}

void Game::GameLoop(float dt) {
	event_.InputFromDevices();
	render_.UpdateRender(dt);
}
