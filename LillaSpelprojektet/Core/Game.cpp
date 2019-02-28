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
			state_ = GAME;
			menu_.StateManager(state_);
		}
		if (in_event.key.code == sf::Keyboard::Enter) {
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
			//OBS!
			//Currently writes pos to terminal
			//std::cout << "X: " << obj_handler_ptr_->GetPlayerPos().x << "Y: " << obj_handler_ptr_->GetPlayerPos().y << " Z: " << obj_handler_ptr_->GetPlayerPos().z << std::endl;
		}
		//Use Ability
		if (in_event.key.code == sf::Keyboard::E) {
			this->obj_handler_ptr_->PlayerUseAbility();
		}
		//Temporary teleport to boss room (OBS! need to connect with interaction)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
			this->obj_handler_ptr_->PlayerTeleport();

		}
		/*---------------End Keyboard inputs-----------------*/

		//Attack
		//goes here


		//-------------------------------------------------------
		//--------------------Game Control-----------------------
		//-------------------------------------------------------
		//Pause
		if (in_event.key.code == sf::Keyboard::Escape) {
			state_ = PAUSE;
			menu_.StateManager(state_);
		}
		//Swap between normal and debug camera
		if (in_event.key.code == sf::Keyboard::O) {
			cam_handler_ptr_->SwapCamera();
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
		//Move debug camera to its default position
		cam_handler_ptr_->SetCameraPos(CAMERA_DEBUG_POSITION_X, CAMERA_DEBUG_POSITION_Y, CAMERA_DEBUG_POSITION_Z);
	}
}

//Public---------------------------------------------------

Game::Game() {
	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(256.0, -256.0f, 0.0f), CAMERA_DEFAULT_ZOOM);
	this->obj_handler_ptr_ = new ObjectHandler();
	
	this->state_ = MENU;
}

Game::~Game() {
	delete this->cam_handler_ptr_;
	delete this->obj_handler_ptr_;
}

void Game::InitializeGame() {
	this->menu_.Initiliaze();
	this->render_.InitializeRender();
	this->obj_handler_ptr_->InitializeObjectHandler(
		render_.GetMapPointer(),
		render_.GetDoorKeyPosition());

	this->game_clock_.restart();	//Get the clock going correctly
}

void Game::GameIteration() {
	
	//Update deltatime
	this->game_deltatime_ = this->game_clock_.restart().asSeconds();

	if (state_ == MENU) {
		render_.RenderMenuState(menu_);
	}
	else if (state_ == GAME) {
		// Create a vector to hold interesting objects
		std::vector<ObjectPackage> object_vector;

		//Update the game logic and fill the vector
		object_vector = this->obj_handler_ptr_->UpdateAndRetrieve(this->game_deltatime_);
		
		//Update the camera's position
		cam_handler_ptr_->SetPrimaryCameraPos(this->obj_handler_ptr_->GetPlayerPos());

		if (this->obj_handler_ptr_->PlayerInBossRoom()) // Swap primary camera to 'boss' camera
			cam_handler_ptr_->SwapCameraToBossCamera();

		PlayerInfoPackage temp_player_data;
		temp_player_data.max_hp = 100;
		temp_player_data.current_hp = 100;
		//TEMP

		//Update the screen
		render_.UpdateRender(
			this->game_deltatime_,
			cam_handler_ptr_->GetCameraPosition(),
			cam_handler_ptr_->GetViewPerspectiveMatrix(),
			object_vector,
			temp_player_data
		);

		/*--------------Restart Game when death occurs--------------*/
		if (temp_player_data.current_hp == 0) { //Use this one
			state_ = DEATH;
		}
		/*----------End Restart Game when death occurs--------------*/
	}
	else if (state_ == PAUSE) {
		render_.RenderPauseMenu(menu_);
	}
	else if (state_ == DEATH) {
		render_.RenderDeathMenu(menu_);
	}
}

void Game::InputEvents(const sf::Event& in_event) {
	
	//This function tracks event input
	//for things that should only trigger once
	//per button press
	
	if (state_ == GAME) {
		this->InputForGameState(in_event);
	}
	else if (state_ == MENU) {
		this->InputForMenuState(in_event);
	}
	else if (state_ == PAUSE) {
		this->InputForPauseState(in_event);
	}
	else if (state_ == DEATH) {
		this->InputForDeathState(in_event);
	}
}

void Game::InputContinual() {

	//This function tracks continual input
	//for things such as player movement sideways

	this->input_deltatime_ = this->input_clock_.restart().asSeconds();

	if (this->state_ != GAME) { return; }

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