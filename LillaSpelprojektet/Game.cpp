#include "game.h"

Game::Game() {
	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(0.0), 20.0);
	this->obj_handler_ptr_ = new ObjectHandler();
}

Game::~Game() {
	delete cam_handler_ptr_;
	delete this->obj_handler_ptr_;
}

void Game::InitializeGame() {
	//render_.InitializeRender();
	this->obj_handler_ptr_->InitializeObjectHandler();
	forwardRender_.HelloScreen();
}

void Game::InputFromDevices(float in_deltatime) {

	/*---------------Keyboard inputs-----------------*/
	//Walk up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//Do something
	}
	//Walk down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//Do something
	}
	//Walk right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//Do something
	}
	//Walk left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//Do something
	}
	//Pick up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		//Do something
	}
	/*---------------End Keyboard inputs-----------------*/

	/*---------------Mouse inputs-----------------*/
	//Attack
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		//Do something
	}
	//Use ability
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		//Do something
	}
	/*---------------End Mouse inputs-----------------*/

	/*---------------Secondary Camera Control-----------------*/
	float cam_speed = 150.0f * in_deltatime;
	bool secondary = cam_handler_ptr_->GetMode();		//Primary is 0 (boolean false), Secondeary is 1 (boolean !false)

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
		//Swap camera (Primary/Secondary)
		cam_handler_ptr_->SwapCamera();
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		//Move active camera upwards
		cam_handler_ptr_->MoveCamera(0.0f, cam_speed);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		//Move active camera leftwards
		cam_handler_ptr_->MoveCamera(-cam_speed, 0.0f);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		//Move active camera rightwards
		cam_handler_ptr_->MoveCamera(cam_speed, 0.0f);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		//Move active camera downwards
		cam_handler_ptr_->MoveCamera(0.0f, -cam_speed);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
		//Move active camera forwards ("zoom in")
		cam_handler_ptr_->MoveCamera(0.0, 0.0, -cam_speed);
	}
	else if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
		//Move active camera backwards ("zoom out")
		cam_handler_ptr_->MoveCamera(0.0, 0.0, cam_speed);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		//Move active camera forwards ("zoom in")
		cam_handler_ptr_->SetCameraPos(0.0f, 0.0f, 5.0);
	}

	/*---------------End Secondary Camera Control-----------------*/
}

void Game::GameLoop(float in_deltatime) {
	InputFromDevices(in_deltatime);
	/*render_.UpdateRender(
		in_deltatime, 
		cam_handler_ptr_->GetCameraPosition(),
		cam_handler_ptr_->GetPerspectiveMatrix(),
		cam_handler_ptr_->GetViewMatrix());*/
	static float c = 0;
	c += 1.0f * in_deltatime;

	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f));
	m = glm::rotate(m, (90.0f + c), glm::vec3(1.0f, 1.0f, 0.0f));
	glm::mat4 v = this->cam_handler_ptr_->GetViewMatrix();
	glm::mat4 p = this->cam_handler_ptr_->GetPerspectiveMatrix();

	glm::mat4 matrix = p * v * m;

	glUniformMatrix4fv(
		glGetUniformLocation(forwardRender_.GetProgramFromShader(), "matrix"), 
		1, 
		GL_FALSE,
		glm::value_ptr(matrix)
	);

	forwardRender_.RenderScreen();

	matrix = glm::mat4(1);
	m = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, -100.0f, 100.0f));
	m = glm::rotate(m, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m = glm::scale(m, glm::vec3(0.3f, 0.3f, 0.6f));
	v = this->cam_handler_ptr_->GetViewMatrix();
	p = this->cam_handler_ptr_->GetPerspectiveMatrix();

	matrix = p * v * m;

	glUniformMatrix4fv(
		glGetUniformLocation(forwardRender_.GetProgramFromShader(), "matrix"),
		1,
		GL_FALSE,
		glm::value_ptr(matrix)
	);

	forwardRender_.RenderMap();
}