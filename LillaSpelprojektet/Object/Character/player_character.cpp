#include "player_character.h"
#include "../../globals.h"

//Private--------------------------------------------------


//Public---------------------------------------------------
PlayerCharacter::PlayerCharacter(glm::vec3 start_pos) : Character(start_pos, OBJECT_ID_PLAYER) {
	this->top_speed_ = PLAYER_TOP_SPEED;
	this->jump_speed_ = PLAYER_JUMP_VELOCITY;

	this->weapon_.id = SWORD;
	//this->ability_ptr_ = new Ability();
	//this->ability_ptr_ = new DoubleJump();
	this->ability_ptr_ = new Dash();
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
	else if (this->ability_ptr_->id_ == ABILITY_DOUBLE_JUMP) {
		//If they do we call for the execution of that ability
		this->ability_ptr_->ExecuteAbility(*this);
	}
	
	return;
}

void PlayerCharacter::UseAbility() {
	this->ability_ptr_->ExecuteAbility(*this);
}

void PlayerCharacter::SetAirborne(bool in_air) {
	this->airborne_ = in_air;

	//If the player just was put out of the air
	//and it has a double jump, set the double jump to available
	if (!in_air && this->ability_ptr_->id_ == ABILITY_DOUBLE_JUMP) {
		((DoubleJump*)this->ability_ptr_)->available_ = true;
	}

}

void PlayerCharacter::UpdateStatus(const float& in_deltatime) {
	
	//Check if the current ability is a cooldown ability
	CooldownClass* cd_class_ptr = dynamic_cast<CooldownClass*>(this->ability_ptr_);
	if (cd_class_ptr != NULL) {
		//If it is, update its cooldown
		cd_class_ptr->UpdateCooldown(in_deltatime);
	}

	/*
	//Update ability cooldowns
	switch (this->ability_ptr_->id_)
	{
	case ABILITY_DASH:
		//WIP:
		//Create more children an different types of ability types
		break;
	default:
		break;
	}
	*/

}