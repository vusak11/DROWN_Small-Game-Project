#include "game.h"

Game::Game() {
	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(0.0), 50.0);
	this->obj_handler_ptr_ = new ObjectHandler();
}

Game::~Game() {
	delete cam_handler_ptr_;
	delete this->obj_handler_ptr_;
}

void Game::InitializeGame() {
	render_.InitializeRender();
	this->obj_handler_ptr_->InitializeObjectHandler();
	//forwardRender_.HelloScreen();
}

void Game::InputFromDevices(float in_deltatime) {

	/*---------------Keyboard inputs-----------------*/
	//Walk up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->obj_handler_ptr_->PlayerJump();
	}
	//Walk down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//Do something
	}
	//Walk right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->obj_handler_ptr_->PlayerMoveRight();
	}
	//Walk left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->obj_handler_ptr_->PlayerMoveLeft();
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
	
	// This updates the player position.
	std::vector<ObjectPackage> object_vector;
	object_vector = this->obj_handler_ptr_->UpdateAndRetrieve();

	render_.UpdateRender(
		in_deltatime,
		cam_handler_ptr_->GetCameraPosition(),
		cam_handler_ptr_->GetViewPerspectiveMatrix(),
		object_vector
	);
}