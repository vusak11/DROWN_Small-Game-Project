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
}
