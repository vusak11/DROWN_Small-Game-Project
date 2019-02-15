#include "player_character.h"

#include "../../globals.h"

//Private--------------------------------------------------


//Public---------------------------------------------------
PlayerCharacter::PlayerCharacter(glm::vec3 start_pos) : Character(start_pos, OBJECT_ID_PLAYER) {
	this->move_speed_ = PLAYER_X_SPEED;
	this->jump_speed_ = PLAYER_Y_SPEED;

	this->weapon_ = SWORD;
	this->ability_ = NONE;

}

PlayerCharacter::~PlayerCharacter() {

}

void PlayerCharacter::MoveLeft() {
	//Set player velocity towards negative x
	glm::vec3 velocity_change = glm::vec3(-this->move_speed_, 0.0f, 0.0f);

	this->AlterVelocityVec(velocity_change);
}

void PlayerCharacter::MoveRight() {
	//Set player velocity towards positive x
	glm::vec3 velocity_change = glm::vec3(this->move_speed_, 0.0f, 0.0f);

	this->AlterVelocityVec(velocity_change);
}

void PlayerCharacter::Jump() {

	//If player is already in the air, don't do anything
	if (this->object_metadata_.airborne) { return; }

	//Set player velocity towards positive y
	glm::vec3 velocity_change = glm::vec3(0.0f, this->jump_speed_, 0.0f);
	this->AlterVelocityVec(velocity_change);

	//Then set the object to be airborne
	this->object_metadata_.airborne = true;
}
