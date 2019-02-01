#include "game.h"

void Game::DrawScene() {
}

Game::Game() {
	
	
}

Game::~Game() {
	delete this->cam_handler_ptr_;
	//delete this->obj_handler_ptr_;
	delete this->event_handler_ptr_;
}

void Game::Initilize() {
	//Create camera handler
	glm::vec3 start_pos = glm::vec3(0.0f);
	float start_dist = 50.0f;
	this->cam_handler_ptr_ = new CameraHandler(start_pos, start_dist);

	//Create object handler
	//TBA

	//Create EventHandler
	this->event_handler_ptr_ = new EventHandler(this->cam_handler_ptr_/*, obj_handler__ptr_)*/);
}

void Game::GameLoop(float dt) {
	this->event_handler_ptr_->InputFromDevices();
}
