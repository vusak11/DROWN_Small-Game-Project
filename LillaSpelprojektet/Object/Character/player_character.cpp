#include "player_character.h"

#include "../../globals.h"

//Private--------------------------------------------------


//Public---------------------------------------------------
PlayerCharacter::PlayerCharacter(glm::vec3 start_pos) : Character(start_pos, OBJECT_ID_PLAYER) {
	this->top_speed_ = PLAYER_TOP_SPEED;
	this->jump_speed_ = PLAYER_JUMP_VELOCITY;

	this->weapon_ = SWORD;
	this->ability_ = NONE;

}

PlayerCharacter::~PlayerCharacter() {

}

void PlayerCharacter::MoveLeft() {

	//Set player velocity towards negative x
	glm::vec3 new_velocity = this->GetVelocityVec();
	
	new_velocity.x = -this->top_speed_;

	this->SetVelocityVec(new_velocity);
}

void PlayerCharacter::MoveRight() {

	//Set player velocity towards positive x
	glm::vec3 new_velocity = this->GetVelocityVec();
	
	new_velocity.x = this->top_speed_;

	this->SetVelocityVec(new_velocity);
}

void PlayerCharacter::Jump() {

	//If player is already in the air, don't do anything
	if (this->object_metadata_.airborne) { return; }

	//Set player velocity towards positive y
	glm::vec3 new_velocity = this->GetVelocityVec();
	
	new_velocity.y = this->jump_speed_;

	this->SetVelocityVec(new_velocity);

	//Then set the object to be airborne
	this->object_metadata_.airborne = true;
}
