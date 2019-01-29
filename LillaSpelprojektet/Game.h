#pragma once
#include <SFML/Window.hpp>

#include "EventHandler.h"
class Game
{
private:
	EventHandler event_;


	void Draw_scene();
public:
	Game();
	~Game();

	void Initilize();
	void Game_loop();
};

