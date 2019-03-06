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
#include "sound_unit.h"

class Game {
private:
	//Variables
	CameraHandler* cam_handler_ptr_;
	ObjectHandler* obj_handler_ptr_;
	SoundUnit sound_unit_game_;	// this variable contains functionality to play overworld songs

	Render render_;

	GameState previous_state_;
	GameState state_;
	Menu menu_;
	
	sf::Clock game_clock_;		//Clock for the game loop (other thread in main.cpp)
	float game_deltatime_;

	sf::Clock input_clock_;		//Clock for input loop (in main body of main.cpp)
	float input_deltatime_;

	//Functions
	void InputForMenuState(const sf::Event& in_event);
	void InputForPauseState(const sf::Event& in_event);
	void InputForOptionsState(const sf::Event& in_event);
	void InputForDeathState(const sf::Event& in_event);

	void InputForGameState(const sf::Event& in_event);
	void InputForSecondaryCamera(const float& in_deltatime);

public:
	Game();
	~Game();

	void InitializeGame();
	void InitializeStartGame();
	void GameIteration();
	
	void InputEvents(const sf::Event& in_event);
	void InputContinual();

	bool IsRunning();
};

#endif // !GAME_H