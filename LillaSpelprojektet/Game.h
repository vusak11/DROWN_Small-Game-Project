#pragma once
#include <SFML/Window.hpp>
#include "shader_handler.h"
#include "event_handler.h"
class Game
{
private:
	EventHandler event_;
	ShaderHandler shader_;

	void DrawScene();
public:
	Game();
	~Game();

	void Initilize();
	void GameLoop(float dt);
};

