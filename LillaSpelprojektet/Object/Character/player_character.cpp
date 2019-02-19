#include "player_character.h"
//#include "ability.h"	//Follow through on forward declaraction
#include "../../globals.h"

//Private--------------------------------------------------


//Public---------------------------------------------------
PlayerCharacter::PlayerCharacter(glm::vec3 start_pos) : Character(start_pos, OBJECT_ID_PLAYER) {
	this->top_speed_ = PLAYER_TOP_SPEED;
	this->jump_speed_ = PLAYER_JUMP_VELOCITY;

	this->weapon_.id = SWORD;
	//this->ability_ = new Ability();
	this->ability_ = new DoubleJump();

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

	//If player is not in the air
	if (!this->airborne_) {
		//Set player velocity towards positive y
		glm::vec3 new_velocity = this->GetVelocityVec();
		new_velocity.y = this->jump_speed_;
		this->SetVelocityVec(new_velocity);
	}
	//We now know the player to be in the air so we check if they have double jump
	else if (this->ability_->id_ == ABILITY_DOUBLE_JUMP) {
		//If they do we call for the execution of that ability
		this->ability_->ExecuteAbility(*this);
	}
	
	return;
}

void PlayerCharacter::SetAirborne(bool in_air) {
	this->airborne_ = in_air;

	//If the player just was put out of the air
	//and it has a double jump, set the double jump to available
	if (!in_air && this->ability_->id_ == ABILITY_DOUBLE_JUMP) {
		((DoubleJump*)this->ability_)->available_ = true;
	}

}