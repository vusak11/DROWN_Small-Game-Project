#include <iostream>

#include <thread>
#include <functional>

#include "Core/Game.h"
#include "GLDebug.h"

#include <SFML/OpenGL.hpp>

#define _CRTDBG_MAP_ALLOC 

void GameLoop(
	const bool& in_running,
	Game& in_game, 
	sf::Clock& in_clock);

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Drown", sf::Style::Default, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);

	window.setActive(true);
	 
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not linking" << std::endl;
	}

	/*----------Start GL Debugging----------*/
	EnableGLDebug(); //Comment this away to stop output
	/*----------End of Start GL Debugging----------*/

	/*----------Variables----------*/
	sf::Clock gameTime;
	Game game;
	bool running = true;
	/*----------End of Variables----------*/
	
	/*-----------Initialize---------------*/
	game.InitializeGame();

	//Start thread iterating game
	std::thread game_thread(
		&GameLoop,
		std::ref(running),
		std::ref(game),
		std::ref(gameTime)
	);
	//

	/*-----------End Initialize---------------*/

	while (running) {
		sf::Event event;
		while (window.pollEvent(event)) {
			/*----------------Only exit window commands-----------*/
			if (event.type == sf::Event::Closed) {
				running = false;
			}
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
			/*----------------Only exit window commands-----------*/
			/*----------------Input from mouse / keyboard---------*/
			game.InputFunction(gameTime.restart().asSeconds(), event);
			/*----------------Input from mouse / keyboard---------*/
		}

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//game.GameLoop(gameTime.restart().asSeconds());

		window.display();
	}

	game_thread.join();	//wait for thread to finish

	return 0;
}

void GameLoop(
	const bool& in_running,
	Game& in_game,
	sf::Clock& in_clock) {

	while (in_running) {
		
		in_game.GameIteration(in_clock.restart().asSeconds());

	}

}