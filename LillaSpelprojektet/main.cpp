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

		game.gameLoop(gameTime.restart().asSeconds());

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
	}


	return 0;
}