#include <iostream>
#include <SFML/Window.hpp>

#include <memory>		//Fixes error with 'sf::VideoMode(...)'

#include "Globals.h"
#include "Game.h"

int main() {

	sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");
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