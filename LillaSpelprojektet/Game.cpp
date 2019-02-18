#include "game.h"

void Game::InputForGame(float in_deltatime) {
	if (state_ == GAME) {
		/*---------------Keyboard inputs-----------------*/
		//Walk up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			this->obj_handler_ptr_->PlayerJump();
		}
		//Walk down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			//Do something
		}
		//Walk right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			this->obj_handler_ptr_->PlayerMoveRight();
		}
		//Walk left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			this->obj_handler_ptr_->PlayerMoveLeft();
		}
		//Pick up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			//Do something
		}
		/*---------------End Keyboard inputs-----------------*/

		/*---------------Mouse inputs-----------------*/
		//Attack
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//Do something
		}
		//Use ability
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			//Do something
		}
		/*---------------End Mouse inputs-----------------*/

		/*---------------Secondary Camera Control-----------------*/
		float cam_speed = 150.0f * in_deltatime;
		bool secondary = cam_handler_ptr_->GetMode();		//Primary is 0 (boolean false), Secondeary is 1 (boolean !false)
	
		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
			//Swap camera (Primary/Secondary)
			cam_handler_ptr_->SwapCamera();
		}*/

		if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			//Move active camera upwards
			cam_handler_ptr_->MoveCamera(0.0f, cam_speed);
		}

		if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			//Move active camera leftwards
			cam_handler_ptr_->MoveCamera(-cam_speed, 0.0f);
		}

		if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			//Move active camera rightwards
			cam_handler_ptr_->MoveCamera(cam_speed, 0.0f);
		}

		if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			//Move active camera downwards
			cam_handler_ptr_->MoveCamera(0.0f, -cam_speed);
		}

		if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
			//Move active camera forwards ("zoom in")
			cam_handler_ptr_->MoveCamera(0.0, 0.0, -cam_speed);
		}
		else if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
			//Move active camera backwards ("zoom out")
			cam_handler_ptr_->MoveCamera(0.0, 0.0, cam_speed);
		}

		if (secondary && sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			//Move active camera forwards ("zoom in")
			cam_handler_ptr_->SetCameraPos(0.0f, 0.0f, 5.0);
		}
	
		/*---------------End Secondary Camera Control-----------------*/
	}
}

Game::Game() {
	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(0.0f), 10.0f);
	this->obj_handler_ptr_ = new ObjectHandler();
    state_ = MENU;
	menu_.Initiliaze();
}

Game::~Game() {
	delete cam_handler_ptr_;
	delete this->obj_handler_ptr_;
}

void Game::InitializeGame() {
	render_.InitializeRender();
	this->obj_handler_ptr_->InitializeObjectHandler();
}

void Game::GameLoop(float in_deltatime) {
	InputForGame(in_deltatime);
	if (state_ == MENU) {
		render_.RenderMenuState(menu_);
	}
	else if (state_ == GAME) {
		// This updates the player position.
		std::vector<ObjectPackage> object_vector;
		object_vector = this->obj_handler_ptr_->UpdateAndRetrieve(in_deltatime);

		render_.UpdateRender(
			in_deltatime,
			cam_handler_ptr_->GetCameraPosition(),
			cam_handler_ptr_->GetViewPerspectiveMatrix(),
			object_vector
		);
	}
	else if (state_ == PAUSE) {
		render_.RenderPauseMenu(menu_);
	}
}

void Game::InputForMenu(float in_deltatime, sf::Event event) {
	if (state_ == MENU) {
		menu_.StateManager(state_);
		switch (event.type) {
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::W) {
				menu_.NavigateUp();
			}
			if (event.key.code == sf::Keyboard::S) {
				menu_.NavigateDown();
			}
			if (event.key.code == sf::Keyboard::Enter) {
				switch (menu_.GetSelectedItemIndex()) {
				case 0:						//START
					state_ = GAME;
					break;
				case 1:						//OPTIONS
					state_ = OPTIONS;
					menu_.StateManager(state_);
					//Do something, change FOV and so on
					break;
				case 2:						//QUIT
					exit(-1);
				}
			}
			break;
		default:
			break;
		}
	}
	else if (state_ == GAME) {
		switch (event.type) {
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::O) {
				cam_handler_ptr_->SwapCamera();
			}
			if (event.key.code == sf::Keyboard::Escape) {
				state_ = PAUSE;
				menu_.StateManager(state_);
			}
			break;
		default:
			break;
		}
	}
	else if (state_ == PAUSE) {
		switch (event.type) {
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::W) {
				menu_.NavigateUp();
			}
			if (event.key.code == sf::Keyboard::S) {
				menu_.NavigateDown();
			}
			if (event.key.code == sf::Keyboard::Enter) {
				switch (menu_.GetSelectedItemIndex()) {
				case 0:						//Continue
					state_ = GAME;
					menu_.StateManager(state_);
					break;
				case 1:						//OPTIONS
					//Save highscore
					break;
				case 2:
					state_ = OPTIONS;
					menu_.StateManager(state_);
					break;
				case 3:						//QUIT
					exit(-1);
				}
			}
			break;
		default:
			break;
		}
	}
}

