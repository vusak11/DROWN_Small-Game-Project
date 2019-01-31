#include <iostream>
#include "game.h"
#include <SFML/OpenGL.hpp>

int main() {

	sf::Window window(sf::VideoMode(1280, 720), "My window", sf::Style::Default, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);

	window.setActive(true);

	/*----------Variabels----------*/
	sf::Clock gameTime;
	Game game;
	bool running = true;
	/*----------End of Variabels----------*/

	/*-----------Initilize---------------*/
	game.InitilizeGame();
	/*-----------End Initilize---------------*/

	while (running) {
		sf::Event event;
		game.GameLoop(gameTime.restart().asSeconds());

		while (window.pollEvent(event)) {
			/*----------------Only exit window commands-----------*/
			if (event.type == sf::Event::Closed) {
				running = false;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				running = false;
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
			/*----------------Only exit window commands-----------*/
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw

		window.display();
	}

	return 0;
}