#include "event_handler.h"

EventHandler::EventHandler(CameraHandler* in_camera_handler_ptr/*, in_object_handler_ptr*/) {
	this->camera_handler_ptr_ = in_camera_handler_ptr;
	//this->obj_handler_ptr_ = in_object_handler_ptr;
}


EventHandler::~EventHandler() {
}

void EventHandler::InputFromDevices(float in_deltatime) {

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
	float cam_speed = 5.0 * in_deltatime;
	bool secondary = this->camera_handler_ptr_->GetMode();		//Primary is 0 (boolean false), Secondeary is 1 (boolean !false)

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
		//Swap camera (Primary/Secondary)
		this->camera_handler_ptr_->SwapCamera();
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		//Move active camera upwards
		this->camera_handler_ptr_->MoveCamera(cam_speed, 0.0f);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		//Move active camera leftwards
		this->camera_handler_ptr_->MoveCamera(0.0f, -cam_speed);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		//Move active camera rightwards
		this->camera_handler_ptr_->MoveCamera(0.0f, cam_speed);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		//Move active camera downwards
		this->camera_handler_ptr_->MoveCamera(-cam_speed, 0.0f);
	}

	if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
		//Move active camera forwards ("zoom in")
		this->camera_handler_ptr_->MoveCamera(0.0, 0.0, -cam_speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
		//Move active camera backwards ("zoom out")
		this->camera_handler_ptr_->MoveCamera(0.0, 0.0, cam_speed);
	}
	/*---------------End Secondary Camera Control-----------------*/
}
