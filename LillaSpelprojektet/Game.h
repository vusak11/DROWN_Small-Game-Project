#pragma once
#include <SFML/Window.hpp>
#include <GLM/glm.hpp>

#include "event_handler.h"
#include "Camera/camera_handler.h"

class Game
{
private:
	CameraHandler* cam_handler_ptr_;
	//ObjectHandler* obj_handler_ptr_;
	EventHandler* event_handler_ptr_;

	void DrawScene();
public:
	Game();
	~Game();

	void Initilize();
	void GameLoop(float dt);
};

