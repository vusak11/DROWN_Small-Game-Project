#include "game.h"

void Game::InputForGame(float in_deltatime) {
	if (state_ == GameState::GAME) {
		/*---------------Keyboard inputs-----------------*/
		//Walk up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			this->obj_handler_ptr_->PlayerJump();		// Used to temporary test in obj handler class
		}
		//Walk down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			//this->obj_handler_ptr_->PlayerUseAbility();	// Used to temporary test in obj handler class
			std::cout << "X: " << obj_handler_ptr_->GetPlayerPos().x << "Y: " << obj_handler_ptr_->GetPlayerPos().y << " Z: " << obj_handler_ptr_->GetPlayerPos().z << std::endl;
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
			//OBS!
			//CURRENTLY BOUND TO USE ABILITY
			this->obj_handler_ptr_->PlayerUseAbility();
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
			cam_handler_ptr_->SetCameraPos(GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_X"),
				GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_Y"),
				GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_Z"));
		}

		/*---------------End Secondary Camera Control-----------------*/
	}
}

Game::Game() {
	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(256.0, -256.0f, 0.0f),
		GlobalSettings::Access()->ValueOf("CAMERA_DEFAULT_ZOOM"));
	this->obj_handler_ptr_ = new ObjectHandler();
	state_ = GameState::MENU;
	menu_.Initiliaze();
}

Game::~Game() {
	delete cam_handler_ptr_;
	delete this->obj_handler_ptr_;
}

void Game::InitializeGame() {
	render_.InitializeRender();
	this->obj_handler_ptr_->InitializeObjectHandler(render_.GetMapPointer());
}

void Game::GameLoop(float in_deltatime) {
	InputForGame(in_deltatime);
	if (state_ == MENU) {
		render_.RenderMenuState(menu_);
	}
	else if (state_ == GameState::GAME) {
		// This updates the player position.
		std::vector<ObjectPackage> object_vector;
		object_vector = this->obj_handler_ptr_->UpdateAndRetrieve(in_deltatime);
		cam_handler_ptr_->SetPrimaryCameraPos(this->obj_handler_ptr_->GetPlayerPos());

		PlayerInfoPackage temp_player_data;
		temp_player_data.max_hp = 100;
		temp_player_data.current_hp = 100;

		render_.UpdateRender(
			in_deltatime,
			cam_handler_ptr_->GetCameraPosition(),
			cam_handler_ptr_->GetViewPerspectiveMatrix(),
			object_vector,
			temp_player_data
		);

		/*--------------Restart Game when death occurs--------------*/
		if (temp_player_data.current_hp == 0) { //Use this one
			state_ = GameState::DEATH;
		}
		/*----------End Restart Game when death occurs--------------*/
	}
	else if (state_ == GameState::PAUSE) {
		render_.RenderPauseMenu(menu_);
	}
	else if (state_ == GameState::DEATH) {
		render_.RenderDeathMenu(menu_);
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
				case 0:						//Start
					state_ = GAME;
					break;
				case 1:						//Options
					//state_ = OPTIONS; //REAL case
					//menu_.StateManager(state_);
					//Do something, change FOV and so on
					break;
				case 2:						//Quit
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
			if (event.key.code == sf::Keyboard::P) {
				GlobalSettings::Access()->UpdateValuesFromFile();
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
			if (event.key.code == sf::Keyboard::Escape) {
				state_ = GAME;
				menu_.StateManager(state_);
			}
			if (event.key.code == sf::Keyboard::Enter) {
				switch (menu_.GetSelectedItemIndex()) {
				case 0:						//Continue
					state_ = GAME;
					menu_.StateManager(state_);
					break;
				case 1:						//Save score
					state_ = DEATH;
					menu_.StateManager(state_);
					//Save highscore
					break;
				case 2:
					//state_ = OPTIONS;		//Options
					//menu_.StateManager(state_);
					break;
				case 3:						//Quit
					exit(-1);
				}
			}
			break;
		default:
			break;
		}
	}
	else if (state_ == DEATH) {
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
				case 0:						//Restart
					system("restartGame.cmd"); // TEST case
					exit(-1);
					break;
				case 1:						//Save score
					//Save highscore
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
}

