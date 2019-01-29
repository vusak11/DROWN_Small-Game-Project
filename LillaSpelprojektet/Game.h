#pragma once
#include <SFML/Window.hpp>

#include "event-handler.h"
class Game
{
private:
	EventHandler event_;


	void DrawScene();
public:
	Game();
	~Game();

	void Initilize();
	void GameLoop();
};

