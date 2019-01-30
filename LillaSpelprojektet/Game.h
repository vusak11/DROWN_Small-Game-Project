#pragma once
#include <SFML/Window.hpp>

#include "event_handler.h"
class Game
{
private:
	EventHandler event_;


	void DrawScene();
public:
	Game();
	~Game();

	void InitilizeGame();
	void GameLoop(float dt);
};

