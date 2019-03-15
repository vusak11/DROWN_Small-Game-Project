#include "game.h"

//Private--------------------------------------------------

void Game::InputForMenuState(const sf::Event& in_event) {
	menu_.StateManager(state_);
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
				state_ = GameState::GAME;
				break;
			case 1:						//Options
				//state_ = OPTIONS; //REAL case
				//menu_.StateManager(state_);
				//Do something, change FOV and so on
				break;
			case 2:						//Quit
				state_ = QUIT;
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
			if (previous_state_ == GameState::BOSS) {
				state_ = GameState::BOSS;
			}
			else {
				state_ = GameState::GAME;
			}
			menu_.StateManager(state_);
		}
		if (in_event.key.code == sf::Keyboard::Enter) {
			switch (menu_.GetSelectedItemIndex()) {
			case 0:						//Continue
				if (previous_state_ == GameState::BOSS) {
					state_ = GameState::BOSS;
				}
				else {
					state_ = GameState::GAME;
				}
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
				state_ = QUIT;
			}
		}
		break;
	default:
		break;
	}
}

void Game::InputForOptionsState(const sf::Event& in_event) {
	//Empty as of now
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
				state_ = QUIT;
				break;
			case 1:						//Save score
				//Save highscore
				break;
			case 2:						//QUIT
				state_ = QUIT;
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
			previous_state_ = state_;
			state_ = PAUSE;
			menu_.StateManager(state_);
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
		//Move active camera forwards ("zoom in")
		cam_handler_ptr_->SetCameraPos(
			GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_X"),
			GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_Y"),
			GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_Z")
		);
	}
}
//Public---------------------------------------------------

Game::Game() {
	this->game_loaded_ = false;

	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(256.0, -256.0f, 0.0f),
		GlobalSettings::Access()->ValueOf("CAMERA_DEFAULT_ZOOM"));
	this->obj_handler_ptr_ = new ObjectHandler();
	this->meta_data_ptr_ = new MetaData();

	this->state_ = MENU;
}

Game::~Game() {
	delete this->cam_handler_ptr_;
	delete this->obj_handler_ptr_;
	delete this->meta_data_ptr_;
}

void Game::InitializeGame() {
	this->menu_.Initiliaze();
	
	this->meta_data_ptr_->Initialize();

	sound_unit_game_.SetMusicFile((char*)"../Resources/Audio/menusong.wav");
	sound_unit_game_.SetVolumeMusic(50);
	sound_unit_game_.PlayMusic();

	this->render_.InitializeRender(meta_data_ptr_);
	this->obj_handler_ptr_->InitializeObjectHandler(
		render_.GetMapPointer(),
		meta_data_ptr_);


	this->game_clock_.restart();	//Get the clock going correctly

	this->game_loaded_ = true;



}

void Game::InitializeStartGame() {
	sound_unit_game_.SetMusicFile((char*)"../Resources/Audio/cavesong.wav");
	sound_unit_game_.SetVolumeMusic(35);
	sound_unit_game_.PlayMusic();
}

void Game::GameIteration() {
	
	//Update deltatime
	this->game_deltatime_ = this->game_clock_.restart().asSeconds();

	if (state_ == GameState::MENU) {
		render_.RenderMenuState(menu_);
	}
	else if (state_ == GameState::GAME) {
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
			player_info
		);

		// INITIALIZE BOSS FIGHT
		if (this->obj_handler_ptr_->PlayerInBossRoom()) { // Swap primary camera to 'boss' camera
			cam_handler_ptr_->SwapCameraToBossCamera();
			state_ = BOSS;
			obj_handler_ptr_->SetPlayerXYZPosForBoss();
			sound_unit_game_.StopMusic();
			sound_unit_game_.SetMusicFile((char*)"../Resources/Audio/disco2.wav");
			sound_unit_game_.SetVolumeMusic(100);
			sound_unit_game_.PlayMusic();
			std::cout << "ENTERING BOSS STATE" << std::endl;
			obj_handler_ptr_->SpawnBoss();
			// SPAWN BOSS
		}

		/*--------------Restart Game when death occurs--------------*/
		if (player_info.current_hp == 0) { //Use this one
			state_ = GameState::DEATH;
		}
		/*----------End Restart Game when death occurs--------------*/
	}
	else if (state_ == GameState::BOSS)
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
			player_info
		);

		/*--------------Restart Game when death occurs--------------*/
		if (player_info.current_hp == 0) { //Use this one
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
	else if (state_ == GameState::QUIT) {
		//This will break the outside loops
	}
}

void Game::InputEvents(const sf::Event& in_event) {
	
	//This function tracks event input
	//for things that should only trigger once
	//per button press
	
	if (state_ == GameState::GAME || state_ == GameState::BOSS) {
		this->InputForGameState(in_event);
	}
	else if (state_ == GameState::MENU) {
		this->InputForMenuState(in_event);
	}
	else if (state_ == GameState::PAUSE) {
		this->InputForPauseState(in_event);
	}
	else if (state_ == GameState::DEATH) {
		this->InputForDeathState(in_event);
	}
}

void Game::InputContinual() {

	//This function tracks continual input
	//for things such as player movement sideways

	this->input_deltatime_ = this->input_clock_.restart().asSeconds();

	if (this->state_ != GameState::GAME && this->state_ != GameState::BOSS) { return; }

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
	return (this->state_ != QUIT);
}

MetaData * Game::getMetaDataPtr() const {
	return meta_data_ptr_;
}
