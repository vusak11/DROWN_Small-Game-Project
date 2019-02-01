#ifndef GAME_H
#define GAME_H


#include <SFML/Window.hpp>
#include <GLM/glm.hpp>

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "shader_handler.h"
#include "event_handler.h"
#include "Camera/camera_handler.h"

#include "render.h"
class Game {
private:
	CameraHandler* cam_handler_ptr_;
	//ObjectHandler* obj_handler_ptr_;
	EventHandler* event_handler_ptr_;
	Render render_;
	ShaderHandler shader_;

	void DrawScene();
public:
	Game();
	~Game();

	void InitilizeGame();
	void GameLoop(float in_deltatime);
};

#endif // !GAME_H