#include "game.h"

void Game::DrawScene() {
}

Game::Game() {
	this->Initilize();
	
}

Game::~Game() {
	delete this->cam_handler_ptr_;
	//delete this->obj_handler_ptr_;
	delete this->event_handler_ptr_;
}

void Game::Initilize() {
	//Create camera handler
	glm::vec3 start_pos = glm::vec3(0.0f);
	float start_distance = 50.0f;
	this->cam_handler_ptr_ = new CameraHandler(start_pos, start_distance);

	//Create object handler
	//TBA

	//Create EventHandler
	this->event_handler_ptr_ = new EventHandler(this->cam_handler_ptr_/*, obj_handler__ptr_)*/);
}

void Game::GameLoop(float in_deltatime) {
	this->event_handler_ptr_->InputFromDevices(in_deltatime);
}
