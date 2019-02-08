#ifndef GAME_H
#define GAME_H


#include <SFML/Window.hpp>
#include <GLM/glm.hpp>

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "Camera/camera_handler.h"

#include "render.h"
class Game {
private:
	enum STATES {
		RENDER,
		PAUSE
	};

	STATES state;

	CameraHandler* cam_handler_ptr_;
	//ObjectHandler* obj_handler_ptr_;
	Render render_;
	void InputFromDevices(float in_deltatime);
public:
	Game();
	~Game();

	void InitializeGame();
	void GameLoop(float in_deltatime);
};

#endif // !GAME_H