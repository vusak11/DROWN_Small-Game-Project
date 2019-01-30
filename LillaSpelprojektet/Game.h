#pragma once

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "shader_handler.h"
#include "event_handler.h"
#include "render.h"
class Game {
private:
	EventHandler event_;
	Render render_;
	ShaderHandler shader_;

	void DrawScene();
public:
	Game();
	~Game();

	void InitilizeGame();
	void GameLoop(float dt);
};

