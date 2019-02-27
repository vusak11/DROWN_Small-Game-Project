#ifndef GAME_H
#define GAME_H


#include <SFML/Window.hpp>
#include <GLM/glm.hpp>
#include "render.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "../UI/menu.h"
#include "../Camera/camera_handler.h"
#include "../Object/object_handler.h"
#include "../global_settings.h"

class Game {
private:
	CameraHandler* cam_handler_ptr_;
	ObjectHandler* obj_handler_ptr_;

	Render render_;

	GameState state_;
	Menu menu_;

	void InputForGame(float in_deltatime);
public:
	Game();
	~Game();

	void InitializeGame();
	void GameLoop(float in_deltatime);
	void InputForMenu(float in_deltatime, sf::Event event);
};

#endif // !GAME_H