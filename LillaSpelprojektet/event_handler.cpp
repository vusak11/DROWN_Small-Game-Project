#include "event_handler.h"


EventHandler::EventHandler() {
}


EventHandler::~EventHandler() {
}

void EventHandler::InputFromDevices() {

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
	float cam_speed = 5.0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
		//Swap camera (Primary/Secondary)
		this->cam_handler_ptr_->SwapCamera();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		//Move active camera upwards
		this->cam_handler_ptr_->MoveCamera(cam_speed, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		//Move active camera leftwards
		this->cam_handler_ptr_->MoveCamera(0.0f, -cam_speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		//Move active camera rightwards
		this->cam_handler_ptr_->MoveCamera(0.0f, cam_speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		//Move active camera downwards
		this->cam_handler_ptr_->MoveCamera(-cam_speed, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
		//Move active camera forwards ("zoom in")
		this->cam_handler_ptr_->MoveCamera(0.0, 0.0, -cam_speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
		//Move active camera backwards ("zoom out")
		this->cam_handler_ptr_->MoveCamera(0.0, 0.0, cam_speed);
	}
	/*---------------End Secondary Camera Control-----------------*/
}
