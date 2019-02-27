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
	//Variables
	CameraHandler* cam_handler_ptr_;
	ObjectHandler* obj_handler_ptr_;

	Render render_;

	GameState state_;
	Menu menu_;

	//std::thread* input_thread_ptr_;		//If using this do gameloop thread instead
	//float* input_thread_deltatime_ptr_;

	//Functions

	void InputForGame(const float& in_deltatime, const sf::Event& in_event);
	void InputForSecondaryCamera(const float& in_deltatime, const sf::Event& in_event);
	
	void InputForGameB(const float& in_deltatime, const sf::Event& in_event);
	
	void InputForSecondaryCameraB(const float& in_deltatime);

	void InputForMenu(const float& in_deltatime, const sf::Event& in_event);
	void InputForPause(const float& in_deltatime, const sf::Event& in_event);
	void InputForOptions(const float& in_deltatime, const sf::Event& in_event);
	void InputForDeath(const float& in_deltatime, const sf::Event& in_event);
public:
	Game();
	~Game();

	void InitializeGame();
	void GameIteration(float in_deltatime);
	
	void InputFunction(const float& in_deltatime, const sf::Event& in_event);

	void InputForGameLoop(const float& in_deltatime);
};

#endif // !GAME_H