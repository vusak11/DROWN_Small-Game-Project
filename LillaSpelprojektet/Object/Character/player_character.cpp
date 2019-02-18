#include "player_character.h"
//#include "ability.h"	//Follow through on forward declaraction
#include "../../globals.h"

//Private--------------------------------------------------


//Public---------------------------------------------------
PlayerCharacter::PlayerCharacter(glm::vec3 start_pos) : Character(start_pos, OBJECT_ID_PLAYER) {
	this->top_speed_ = PLAYER_TOP_SPEED;
	this->jump_speed_ = PLAYER_JUMP_VELOCITY;

	this->weapon_.id = SWORD;
	this->ability_->id_ = NONE;

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

	//If player is in the air
	if (this->airborne_) {
		//If the player do not have double jump return
		if (this->ability_->id_ != DOUBLE_JUMP) { return; }
		
		//If the player has double jump available
		//DO DOUBLE JUMP
	}
	
	//Set player velocity towards positive y
	glm::vec3 new_velocity = this->GetVelocityVec();
	
	new_velocity.y = this->jump_speed_;

	this->SetVelocityVec(new_velocity);
}

void PlayerCharacter::SetAirborne(bool in_bool) {
	this->airborne_ = in_bool;
}