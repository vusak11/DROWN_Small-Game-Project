#include <iostream>
#include <SFML/Window.hpp>

#include "Game.h"

int main() {

	sf::Window window(sf::VideoMode(1280, 720), "My window");
	Game game;

	while (window.isOpen()) {
		sf::Event event;

		game.gameLoop();

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
	}


	return 0;
}