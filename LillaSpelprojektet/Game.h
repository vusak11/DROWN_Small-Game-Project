#ifndef GAME_H
#define GAME_H

#include <SFML/Window.hpp>
#include <GLM/glm.hpp>

#include <GL/glew.h>
#include "Camera/camera_handler.h"
#include "forwardRendering.h"

#include "render.h"

class Game {
private:
	CameraHandler* cam_handler_ptr_;
	//ObjectHandler* obj_handler_ptr_;
	Render render_;
	ForwardRendering forwardRender_;
	void InputFromDevices(float in_deltatime);
public:
	Game();
	~Game();

	void InitializeGame();
	void GameLoop(float in_deltatime);
};

#endif // !GAME_H