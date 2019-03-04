#include <iostream>

#include <thread>

#include "Core/Game.h"
#include "GLDebug.h"

#include <SFML/OpenGL.hpp>

#define _CRTDBG_MAP_ALLOC 

void GameLoop(
	const bool& in_running,
	Game& in_game,
	sf::Window& in_window);

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
	//EnableGLDebug(); //Comment this away to stop output
	/*----------End of Start GL Debugging----------*/

	/*----------Variables----------*/
	Game game;
	bool running = true;
	/*----------End of Variables----------*/
	
	/*-----------Initialize---------------*/
	
	//Start the game
	game.InitializeGame();

	//Start thread iterating game
	std::thread game_thread(
		&GameLoop,
		std::ref(running),
		std::ref(game),
		std::ref(window)
	);

	//Tell this thread to not use the window actively
	window.setActive(false);

	//Turn of repeating key presses
	window.setKeyRepeatEnabled(false);

	/*-----------End Initialize---------------*/

	while (game.IsRunning()) {
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
			game.InputEvents(event);
			/*----------------Input from mouse / keyboard---------*/
		}

		game.InputContinual();
	}

	game_thread.join();	//wait for thread to finish

	return 0;
}

void GameLoop(
	const bool& in_running,
	Game& in_game,
	sf::Window& in_window) {

	while (in_game.IsRunning()) {
		in_game.GameIteration();
		in_window.display();
	}
}