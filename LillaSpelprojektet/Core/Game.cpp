#include "game.h"

//Private--------------------------------------------------

void Game::InputForGame(const float& in_deltatime, const sf::Event& in_event) {
	switch (in_event.type) {
		case sf::Event::KeyPressed:
		//-------------------------------------------------------
		//--------------------Player Control---------------------
		//-------------------------------------------------------
		//Walk up
		if (in_event.key.code == sf::Keyboard::W) {		
			this->obj_handler_ptr_->PlayerJump();
		}
		//Walk right
		if (in_event.key.code == sf::Keyboard::D) {
			this->obj_handler_ptr_->PlayerMoveRight();
		}
		//Walk left
		if (in_event.key.code == sf::Keyboard::A) {
			this->obj_handler_ptr_->PlayerMoveLeft();
		}
		//Pick up
		if (in_event.key.code == sf::Keyboard::S) {
			//OBS!
			//Currently writes pos to terminal
			std::cout << "X: " << obj_handler_ptr_->GetPlayerPos().x << "Y: " << obj_handler_ptr_->GetPlayerPos().y << " Z: " << obj_handler_ptr_->GetPlayerPos().z << std::endl;
		}
		//Use Ability
		if (in_event.key.code == sf::Keyboard::E) {
			this->obj_handler_ptr_->PlayerUseAbility();
		}

		//Attack
		//goes here
		
		//-------------------------------------------------------
		//---------------Secondary Camera Control----------------
		//-------------------------------------------------------
		//Primary is 0 (boolean false), Secondary is 1 (boolean !false)
		//bool secondary = cam_handler_ptr_->GetMode();
		if (cam_handler_ptr_->GetMode()) {
			this->InputForSecondaryCamera(in_deltatime, in_event);
		}

		break;
	case sf::Event::KeyReleased:
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

void Game::InputForSecondaryCamera(const float& in_deltatime, const sf::Event& in_event) {
	float cam_speed = 150.0f * in_deltatime;
	
	if (in_event.key.code == sf::Keyboard::Up) {
		//Move active camera upwards
		cam_handler_ptr_->MoveCamera(0.0f, cam_speed);
	}

	if (in_event.key.code == sf::Keyboard::Left) {
		//Move active camera leftwards
		cam_handler_ptr_->MoveCamera(-cam_speed, 0.0f);
	}

	if (in_event.key.code == sf::Keyboard::Right) {
		//Move active camera rightwards
		cam_handler_ptr_->MoveCamera(cam_speed, 0.0f);
	}

	if (in_event.key.code == sf::Keyboard::Down) {
		//Move active camera downwards
		cam_handler_ptr_->MoveCamera(0.0f, -cam_speed);
	}

	if (in_event.key.code == sf::Keyboard::Add) {
		//Move active camera forwards ("zoom in")
		cam_handler_ptr_->MoveCamera(0.0, 0.0, -cam_speed);
	}
	else if (in_event.key.code == sf::Keyboard::Subtract) {
		//Move active camera backwards ("zoom out")
		cam_handler_ptr_->MoveCamera(0.0, 0.0, cam_speed);
	}

	if (in_event.key.code == sf::Keyboard::P) {
		//Move debug camera to its default position
		cam_handler_ptr_->SetCameraPos(CAMERA_DEBUG_POSITION_X, CAMERA_DEBUG_POSITION_Y, CAMERA_DEBUG_POSITION_Z);
	}
}

void Game::InputForGameB(const float& in_deltatime, const sf::Event& in_event) {

	this->InputForGameLoop(in_deltatime);

	switch (in_event.type) {
	case sf::Event::KeyReleased:
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

void Game::InputForGameLoop(const float& in_deltatime) {

	if (this->state_ != GAME) { return; }
	
	//-------------------------------------------------------
	//--------------------Player Control---------------------
	//-------------------------------------------------------
	//Walk up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->obj_handler_ptr_->PlayerJump();
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//OBS!
		//Currently writes pos to terminal
		std::cout << "X: " << obj_handler_ptr_->GetPlayerPos().x << "Y: " << obj_handler_ptr_->GetPlayerPos().y << " Z: " << obj_handler_ptr_->GetPlayerPos().z << std::endl;
	}
	//Use Ability
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		this->obj_handler_ptr_->PlayerUseAbility();
	}

	//Attack
	//goes here

	//-------------------------------------------------------
	//---------------Secondary Camera Control----------------
	//-------------------------------------------------------
	//Primary is 0 (boolean false), Secondary is 1 (boolean !false)
	//bool secondary = cam_handler_ptr_->GetMode();
	if (cam_handler_ptr_->GetMode()) {
		this->InputForSecondaryCameraB(in_deltatime);
	}
}

void Game::InputForSecondaryCameraB(const float& in_deltatime) {
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

void Game::InputForMenu(const float& in_deltatime, const sf::Event& in_event) {
	menu_.StateManager(state_);
	switch (in_event.type) {
	case sf::Event::KeyReleased:
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

void Game::InputForPause(const float& in_deltatime, const sf::Event& in_event) {
	switch (in_event.type) {
	case sf::Event::KeyReleased:
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

void Game::InputForOptions(const float& in_deltatime, const sf::Event& in_event) {
	//Empty as of now
}

void Game::InputForDeath(const float& in_deltatime, const sf::Event& in_event) {
	switch (in_event.type) {
	case sf::Event::KeyReleased:
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

//Public---------------------------------------------------

Game::Game() {
	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(256.0, -256.0f, 0.0f), CAMERA_DEFAULT_ZOOM);
	this->obj_handler_ptr_ = new ObjectHandler();
	
	this->state_ = MENU;

	//*(this->input_thread_deltatime_ptr_) = 0.0;
}

Game::~Game() {
	delete this->cam_handler_ptr_;
	delete this->obj_handler_ptr_;
	
	//End thread
	//if (this->input_thread_ptr_->joinable()) {
	//	this->input_thread_ptr_->join();
	//}
	//delete this->input_thread_ptr_;
	//
}

void Game::InitializeGame() {
	this->menu_.Initiliaze();
	this->render_.InitializeRender();
	this->obj_handler_ptr_->InitializeObjectHandler(render_.GetMapPointer());

	//Start thread
	//this->input_thread_ptr_ = new std::thread(this->input_thread_deltatime_ptr_);
	//
}

void Game::GameIteration(float in_deltatime) {
	
	//Update the in argument to the input thread
	//*(this->input_thread_deltatime_ptr_) = in_deltatime;
	//

	if (state_ == MENU) {
		render_.RenderMenuState(menu_);
	}
	else if (state_ == GAME) {
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

void Game::InputFunction(const float& in_deltatime, const sf::Event& in_event) {
	if (state_ == GAME) {
		this->InputForGameB(in_deltatime, in_event);
	}
	else if (state_ == MENU) {
		this->InputForMenu(in_deltatime, in_event);
	}
	else if (state_ == PAUSE) {
		this->InputForPause(in_deltatime, in_event);
	}
	else if (state_ == DEATH) {
		this->InputForDeath(in_deltatime, in_event);
	}
}