#include "game.h"

//Private--------------------------------------------------

void Game::InputForMenuState(const sf::Event& in_event) {
	menu_.StateManager(this->previous_states_.back());
	switch (in_event.type) {
	case sf::Event::KeyPressed:
		if (in_event.key.code == sf::Keyboard::W
			|| in_event.key.code == sf::Keyboard::Up) {
			menu_.NavigateUp();
		}
		if (in_event.key.code == sf::Keyboard::S
			|| in_event.key.code == sf::Keyboard::Down) {
			menu_.NavigateDown();
		}
		if (in_event.key.code == sf::Keyboard::Enter) {
			switch (menu_.GetSelectedItemIndex()) {
			case 0:						//Start
				InitializeStartGame();
				this->previous_states_.push_back(GameState::GAME);
				break;
			case 1:						//Options
				this->previous_states_.push_back(GameState::OPTIONS); //REAL case
				menu_.StateManager(this->previous_states_.back());
				//Do something, change FOV and so on
				break;
			case 2:						//Quit
				this->previous_states_.push_back(GameState::QUIT);
			}
		}
		break;
	default:
		break;
	}
}

void Game::InputForPauseState(const sf::Event& in_event) {
	switch (in_event.type) {
	case sf::Event::KeyPressed:
		if (in_event.key.code == sf::Keyboard::W
			|| in_event.key.code == sf::Keyboard::Up) {
			menu_.NavigateUp();
		}
		if (in_event.key.code == sf::Keyboard::S
			|| in_event.key.code == sf::Keyboard::Down) {
			menu_.NavigateDown();
		}
		if (in_event.key.code == sf::Keyboard::Escape) {
			this->previous_states_.pop_back();
			menu_.StateManager(this->previous_states_.back());
		}
		if (in_event.key.code == sf::Keyboard::Enter) {
			switch (menu_.GetSelectedItemIndex()) {
			case 0:						//Continue
				this->previous_states_.pop_back();
				menu_.StateManager(this->previous_states_.back());
				break;
			case 1:						//Save score
				this->previous_states_.push_back(GameState::DEATH);
				menu_.StateManager(this->previous_states_.back());
				menu_.SetSelectedItemIndex(0);

				//Save highscore

				break;
			case 2:						//Options
				this->previous_states_.push_back(GameState::OPTIONS);
				menu_.StateManager(this->previous_states_.back());
				break;
			case 3:						//Quit
				this->previous_states_.push_back(GameState::QUIT);
			}
		}
		break;
	default:
		break;
	}
}

void Game::InputForOptionsState(const sf::Event& in_event) {
	switch (in_event.type) {
	case sf::Event::KeyPressed:
		if (in_event.key.code == sf::Keyboard::W
			|| in_event.key.code == sf::Keyboard::Up) {
			menu_.NavigateUp();
		}
		if (in_event.key.code == sf::Keyboard::S
			|| in_event.key.code == sf::Keyboard::Down) {
			menu_.NavigateDown();
		}
		if (in_event.key.code == sf::Keyboard::Escape) {
			this->previous_states_.pop_back();
			menu_.StateManager(this->previous_states_.back());
		}
		switch (menu_.GetSelectedItemIndex()) {
		case 0:
			//Alter camera zoom using left and right navigation keys
			if (in_event.key.code == sf::Keyboard::A
				|| in_event.key.code == sf::Keyboard::Left) {
				cam_handler_ptr_->MoveCamera(0.0, 0.0, -5.0);
				if (cam_handler_ptr_->GetCameraPosition().z < 20) {
					cam_handler_ptr_->SetCameraPos(cam_handler_ptr_->GetCameraPosition().x, cam_handler_ptr_->GetCameraPosition().y, 20.0);
				}
			}
			if (in_event.key.code == sf::Keyboard::D
				|| in_event.key.code == sf::Keyboard::Right) {
				cam_handler_ptr_->MoveCamera(0.0, 0.0, 5.0);
				if (cam_handler_ptr_->GetCameraPosition().z > 40) {
					cam_handler_ptr_->SetCameraPos(cam_handler_ptr_->GetCameraPosition().x, cam_handler_ptr_->GetCameraPosition().y, 40.0);
				}
			}
			break;
		case 1:
			//Alter volume using left and right navigation keys
			if (in_event.key.code == sf::Keyboard::A
				|| in_event.key.code == sf::Keyboard::Left) {
				sf::Listener::setGlobalVolume(sf::Listener::getGlobalVolume() - 5);
				if (sf::Listener::getGlobalVolume() < 0) {
					sf::Listener::setGlobalVolume(0);
				}
			}
			if (in_event.key.code == sf::Keyboard::D
				|| in_event.key.code == sf::Keyboard::Right) {
				sf::Listener::setGlobalVolume(sf::Listener::getGlobalVolume() + 5);
				if (sf::Listener::getGlobalVolume() > 100) {
					sf::Listener::setGlobalVolume(100);
				}
			}
			break;
		case 2:
			if (in_event.key.code == sf::Keyboard::A || in_event.key.code == sf::Keyboard::D) {
				if (menu_.IsMinIMapEnabled()) {
					menu_.SetMiniMap(false);
				}
				else {
					menu_.SetMiniMap(true);
				}
			}
			break;
		case 3:
			if (in_event.key.code == sf::Keyboard::Enter) {
				this->previous_states_.pop_back();
				menu_.StateManager(this->previous_states_.back());
				menu_.SetSelectedItemIndex(0);
			}
			break;
		default:
			break;
		}
	default:
		break;
	}
}

void Game::InputForDeathState(const sf::Event& in_event) {
	switch (in_event.type) {
	case sf::Event::KeyPressed:
		if (in_event.key.code == sf::Keyboard::W
			|| in_event.key.code == sf::Keyboard::Up) {
			menu_.NavigateUp();
		}
		if (in_event.key.code == sf::Keyboard::S
			|| in_event.key.code == sf::Keyboard::Down) {
			menu_.NavigateDown();
		}
		if (in_event.key.code == sf::Keyboard::Enter) {
			switch (menu_.GetSelectedItemIndex()) {
			case 0:						//Restart
				system("restartGame.cmd"); // TEST case
				this->previous_states_.push_back(GameState::QUIT);
				break;
			case 1:						//Save score
				//Save highscore
				break;
			case 2:						//QUIT
				this->previous_states_.push_back(GameState::QUIT);
			}
		}
		break;
	default:
		break;
	}
}

void Game::InputForGameState(const sf::Event& in_event) {

	switch (in_event.type) {
	case sf::Event::KeyPressed:

		//-------------------------------------------------------
		//--------------------Player Control---------------------
		//-------------------------------------------------------
		//Jump
		if (in_event.key.code == sf::Keyboard::W) {
			this->obj_handler_ptr_->PlayerJump();
		}
		//Pick up
		if (in_event.key.code == sf::Keyboard::S) {
			this->obj_handler_ptr_->PlayerPickUp();
		}
		//Use Ability
		if (in_event.key.code == sf::Keyboard::LShift) {
			this->obj_handler_ptr_->PlayerUseAbility();
		}
		//Attack
		if (in_event.key.code == sf::Keyboard::Space) {
			this->obj_handler_ptr_->PlayerAttack();
		}
		/*---------------End Keyboard inputs-----------------*/

		//Attack
		//goes here


		//-------------------------------------------------------
		//--------------------Game Control-----------------------
		//-------------------------------------------------------
		//Pause
		if (in_event.key.code == sf::Keyboard::Escape) {
			this->previous_states_.push_back(GameState::PAUSE);
			menu_.StateManager(this->previous_states_.back());
		}
		//Swap between normal and debug camera
		if (in_event.key.code == sf::Keyboard::O) {
			cam_handler_ptr_->SwapCamera();
		}
		if (in_event.key.code == sf::Keyboard::L) {
			GlobalSettings::Access()->UpdateValuesFromFile();
		}
		if (in_event.key.code == sf::Keyboard::B)
		{
			obj_handler_ptr_->SetPlayerXYZPosForBoss();
		}

	default:
		break;
	}
}

void Game::InputForSecondaryCamera(const float& in_deltatime) {
	float cam_speed = 150.0f * in_deltatime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		//Move active camera upwards
		cam_handler_ptr_->MoveCamera(0.0f, cam_speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		//Move active camera leftwards
		cam_handler_ptr_->MoveCamera(-cam_speed, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		//Move active camera rightwards
		cam_handler_ptr_->MoveCamera(cam_speed, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		//Move active camera downwards
		cam_handler_ptr_->MoveCamera(0.0f, -cam_speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
		//Move active camera forwards ("zoom in")
		cam_handler_ptr_->MoveCamera(0.0, 0.0, -cam_speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
		//Move active camera backwards ("zoom out")
		cam_handler_ptr_->MoveCamera(0.0, 0.0, cam_speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		//Move active camera to player's position
		//cam_handler_ptr_->SetCameraPos(
		//	GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_X"),
		//	GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_Y"),
		//	GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_Z")
		//);
		cam_handler_ptr_->SetCameraPos(
			this->obj_handler_ptr_->RetrievePlayerInfoPackage().position.x,
			this->obj_handler_ptr_->RetrievePlayerInfoPackage().position.y,
			this->obj_handler_ptr_->RetrievePlayerInfoPackage().position.z + 50
		);
	}
}
//Public---------------------------------------------------

Game::Game() {
	this->game_loaded_ = false;

	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(256.0, -256.0f, 0.0f),
		GlobalSettings::Access()->ValueOf("CAMERA_DEFAULT_ZOOM"));
	this->obj_handler_ptr_ = new ObjectHandler();
	this->sound_unit_ptr_ = new SoundUnit();
	this->meta_data_ptr_ = new MetaData();

	//this->state_ = MENU;
	this->previous_states_.push_back(GameState::MENU);
}

Game::~Game() {
	delete this->cam_handler_ptr_;
	delete this->obj_handler_ptr_;
	delete this->sound_unit_ptr_;
	delete this->meta_data_ptr_;
}

void Game::InitializeGame() {
	this->meta_data_ptr_->Initialize();
	
	this->menu_.Initialize();
	
	this->render_.InitializeRender(meta_data_ptr_);
	this->obj_handler_ptr_->InitializeObjectHandler(
		render_.GetMapPointer(),
		meta_data_ptr_);

	sound_unit_ptr_->SetMusicFile((char*)"../Resources/Audio/menusong.wav");
	sound_unit_ptr_->PlayMusic();

	this->game_clock_.restart();	//Get the clock going correctly

	this->game_loaded_ = true;



}

void Game::InitializeStartGame() {
	sound_unit_ptr_->SetMusicFile((char*)"../Resources/Audio/cavesong.wav");
	sound_unit_ptr_->PlayMusic();
}

void Game::GameIteration() {
	
	//Update deltatime
	this->game_deltatime_ = this->game_clock_.restart().asSeconds();

	if (this->previous_states_.back() == GameState::MENU) {
		render_.RenderMenuState(menu_);
	}
	else if (this->previous_states_.back() == GameState::GAME) {
		// Create a vector to hold interesting objects
		std::vector<ObjectPackage> object_vector;

		//Update the game logic and fill the vector
		object_vector = this->obj_handler_ptr_->UpdateAndRetrieve(this->game_deltatime_);
		
		//Get data about the player
		PlayerInfoPackage player_info = this->obj_handler_ptr_->RetrievePlayerInfoPackage();

		//Update the camera's position
		cam_handler_ptr_->SetPrimaryCameraPos(player_info.position);

		//Update the screen
		render_.UpdateRender(
			this->game_deltatime_,
			cam_handler_ptr_->GetCameraPosition(),
			cam_handler_ptr_->GetViewPerspectiveMatrix(),
			object_vector,
			player_info,
			menu_.IsMinIMapEnabled()
		);

		// INITIALIZE BOSS FIGHT
		if (this->obj_handler_ptr_->PlayerInBossRoom()) { // Swap primary camera to 'boss' camera
			cam_handler_ptr_->SwapCameraToBossCamera();
			this->previous_states_.push_back(GameState::BOSS);
			obj_handler_ptr_->SetPlayerXYZPosForBoss();
			sound_unit_ptr_->StopMusic();
			sound_unit_ptr_->SetMusicFile((char*)"../Resources/Audio/disco2.wav");
			sound_unit_ptr_->PlayMusic();
			std::cout << "ENTERING BOSS STATE" << std::endl;
			obj_handler_ptr_->SpawnBoss();
			// SPAWN BOSS
		}

		/*--------------Restart Game when death occurs--------------*/
		if (player_info.current_hp == 0) { //Use this one
			this->previous_states_.push_back(GameState::DEATH);
		}
		/*----------End Restart Game when death occurs--------------*/
	}
	else if (this->previous_states_.back() == GameState::BOSS)
	{
		// Create a vector to hold interesting objects
		std::vector<ObjectPackage> object_vector;

		//Update the game logic and fill the vector
		object_vector = this->obj_handler_ptr_->UpdateAndRetrieve(this->game_deltatime_);

		//Get data about the player
		PlayerInfoPackage player_info = this->obj_handler_ptr_->RetrievePlayerInfoPackage();

		//Update the screen
		render_.UpdateRender(
			this->game_deltatime_,
			cam_handler_ptr_->GetCameraPosition(),
			cam_handler_ptr_->GetViewPerspectiveMatrix(),
			object_vector,
			player_info,
			menu_.IsMinIMapEnabled()
		);

		/*--------------Restart Game when death occurs--------------*/
		if (player_info.current_hp == 0) { //Use this one
			this->previous_states_.push_back(GameState::DEATH);
		}
		/*----------End Restart Game when death occurs--------------*/
	}
	else if (this->previous_states_.back() == GameState::PAUSE) {
		render_.RenderPauseMenu(menu_);
	}
	else if (this->previous_states_.back() == GameState::OPTIONS) {
		render_.RenderOptionsMenu(menu_, cam_handler_ptr_);
	}
	else if (this->previous_states_.back() == GameState::DEATH) {
		render_.RenderDeathMenu(menu_);
	}
	else if (this->previous_states_.back() == GameState::QUIT) {
		//This will break the outside loops
	}
}

void Game::InputEvents(const sf::Event& in_event) {
	
	//This function tracks event input
	//for things that should only trigger once
	//per button press
	
	if (this->previous_states_.back() == GameState::GAME || this->previous_states_.back() == GameState::BOSS) {
		this->InputForGameState(in_event);
	}
	else if (this->previous_states_.back() == GameState::MENU) {
		this->InputForMenuState(in_event);
	}
	else if (this->previous_states_.back() == GameState::PAUSE) {
		this->InputForPauseState(in_event);
	}
	else if (this->previous_states_.back() == GameState::OPTIONS) {
		this->InputForOptionsState(in_event);
	}
	else if (this->previous_states_.back() == GameState::DEATH) {
		this->InputForDeathState(in_event);
	}
}

void Game::InputContinual() {
	//This function tracks continual input
	//for things such as player movement sideways

	this->input_deltatime_ = this->input_clock_.restart().asSeconds();

	if (this->previous_states_.back() != GameState::GAME && this->previous_states_.back() != GameState::BOSS) { return; }

	//-------------------------------------------------------
	//--------------------Player Control---------------------
	//-------------------------------------------------------
	//Walk right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->obj_handler_ptr_->PlayerMoveRight();
	}
	//Walk left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->obj_handler_ptr_->PlayerMoveLeft();
	}

	//-------------------------------------------------------
	//---------------Secondary Camera Control----------------
	//-------------------------------------------------------
	//Primary is 0 (boolean false), Secondary is 1 (boolean !false)
	//bool secondary = cam_handler_ptr_->GetMode();
	if (cam_handler_ptr_->GetMode()) {
		this->InputForSecondaryCamera(this->input_deltatime_);
	}
}

bool Game::IsLoaded() {
	return this->game_loaded_;
}

bool Game::IsRunning() {
	//If the game ain't quittin', it's runnin'
	return (this->previous_states_.back() != QUIT);
}