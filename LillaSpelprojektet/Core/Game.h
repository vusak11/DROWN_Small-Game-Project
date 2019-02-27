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

class Game {
private:
	CameraHandler* cam_handler_ptr_;
	ObjectHandler* obj_handler_ptr_;

	Render render_;

	GameState state_;
	Menu menu_;

	void InputForGame(const float& in_deltatime, const sf::Event& in_event);
	void InputForSecondaryCamera(const float& in_deltatime, const sf::Event& in_event);
	void InputForMenu(const float& in_deltatime, const sf::Event& in_event);
	void InputForPause(const float& in_deltatime, const sf::Event& in_event);
	void InputForOptions(const float& in_deltatime, const sf::Event& in_event);
	void InputForDeath(const float& in_deltatime, const sf::Event& in_event);
public:
	Game();
	~Game();

	void InitializeGame();
	void GameLoop(float in_deltatime);
	
	void InputFunction(const float& in_deltatime, const sf::Event& in_event);
};

#endif // !GAME_H