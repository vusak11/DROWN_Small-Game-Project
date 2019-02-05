#include "game.h"

void Game::DrawScene() {
}

Game::Game() {
	//Create camera handler
	glm::vec3 start_pos = glm::vec3(0.0f);
	float start_distance = 50.0f;
	this->cam_handler_ptr_ = new CameraHandler(start_pos, start_distance);

	//Create object handler
	this->obj_handler_ptr_ = new ObjectHandler();

	//Create EventHandler
	this->event_handler_ptr_ = new EventHandler(this->cam_handler_ptr_/*, obj_handler__ptr_)*/);

}

Game::~Game() {
	delete this->cam_handler_ptr_;
	delete this->obj_handler_ptr_;
	delete this->event_handler_ptr_;
}

void Game::InitilizeGame() {
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not linking" << std::endl;
	}

	shader_.Initialize("glsl/forward_vs.glsl", "glsl/forward_fs.glsl");

	this->obj_handler_ptr_->TestObjectHandler();		//NTS: Just for testing
}

void Game::GameLoop(float in_deltatime) {
	this->event_handler_ptr_->InputFromDevices(in_deltatime);
	render_.UpdateRender(in_deltatime);
}
