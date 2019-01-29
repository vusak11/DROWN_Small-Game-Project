#include <iostream>
#include <SFML/Window.hpp>

#include "Game.h"

int main() {

	sf::Window window(sf::VideoMode(1280, 720), "My window");

	/*----------Variabels----------*/
	sf::Clock gameTime;
	Game game;

	/*----------End of Variabels----------*/

	while (window.isOpen()) {
		sf::Event event;

		game.GameLoop();

		while (window.pollEvent(event)) {

			/*----------Only window close commands---------------*/
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
			}
			/*----------Only window close commands---------------*/
		}
	}


	return 0;
}