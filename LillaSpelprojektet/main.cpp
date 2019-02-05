#include <crtdbg.h>

#include "game.h"
#include "UI/start_menu.h"
#include <iostream>
#include <SFML/OpenGL.hpp>

#define _CRTDBG_MAP_ALLOC 

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	sf::RenderWindow window(sf::VideoMode(1280, 720), "My window", sf::Style::Default, sf::ContextSettings(32));

	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not linking" << std::endl;
	}

	/*----------Variables----------*/
	StartMenu main_menu;
	sf::Clock gameTime;
	Game game;
	bool running = true;
	/*----------End of Variables----------*/
	
	/*-----------Initialize---------------*/
	game.InitializeGame();
	/*-----------End Initialize---------------*/

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
		glBegin(GL_TRIANGLES);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		window.clear();

		// Draw SFML
		window.pushGLStates();
		main_menu.Draw(window);
		window.popGLStates();

		// Display rendered frame
		window.display();
		glEnd();
	}

	return 0;
}