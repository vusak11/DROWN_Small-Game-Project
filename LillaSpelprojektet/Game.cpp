#include "game.h"

Game::Game() {
	this->cam_handler_ptr_ = new CameraHandler(glm::vec3(100.0f, -200.0f, -80.0f), 80.0f);
	this->obj_handler_ptr_ = new ObjectHandler();
	menu_.Initiliaze();
    state_ = GAME;
}

Game::~Game() {
	delete cam_handler_ptr_;
	delete this->obj_handler_ptr_;
}

void Game::InitializeGame() {
	render_.InitializeRender();
	this->obj_handler_ptr_->InitializeObjectHandler(render_.getMapPointer(0)->GetTempHeightList());
}

void Game::InputFromDevices(float in_deltatime) {
	if (state_ == MENU) {
		// Navigate up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		menu_.NavigateUp();
		}
		// Navigate down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		menu_.NavigateDown();
		}
		// Choose selected item
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			switch (menu_.selected_item_index_) {
			case 0:						//START
				state_ = GAME;
				break;
			case 1:						//OPTIONS
				break;
			case 2:						//QUIT
				exit(-1);
			}
		}
	}
	else if (state_ == GAME) {
		/*---------------Keyboard inputs-----------------*/
		//Walk up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			this->obj_handler_ptr_->PlayerJump();		// Used to temporary test in obj handler class
		}
		//Walk down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			this->obj_handler_ptr_->PlayerUseAbility();	// Used to temporary test in obj handler class
			std::cout << "Player y position: " << obj_handler_ptr_->GetPlayerPos().y << std::endl;
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
			//Swap camera (Primary/Secondary)
			cam_handler_ptr_->SwapCamera();
		}

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

void Game::GameLoop(float in_deltatime) {
	InputFromDevices(in_deltatime);
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
}