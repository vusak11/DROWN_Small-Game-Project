#include "player_character.h"

//Private--------------------------------------------------
void PlayerCharacter::AlterVelocity() {
	//Get the player's current velocity
	glm::vec3 velocity = this->GetVelocityVec();

	//Then alter the velocity towards the direction the player is facing
	velocity.x += this->move_acceleration_*this->looking_towards_x_;

	//If velocity exceeds top_speed clamp it down
	float positive_velocity = velocity.x * this->looking_towards_x_;
	if (positive_velocity > this->move_top_speed_) { 
		velocity.x = this->move_top_speed_ * this->looking_towards_x_;
	}

	//Finally set velocity
	this->SetVelocityVec(velocity);
}


//Public---------------------------------------------------
PlayerCharacter::PlayerCharacter(glm::vec3 start_pos)
	: Character(start_pos, OBJECT_ID_PLAYER, PLAYER_START_HP, PLAYER_START_HP) {
	this->move_top_speed_ = PLAYER_MOVE_VELOCITY;
	this->move_acceleration_ = PLAYER_MOVE_ACCELERATION;
	this->jump_speed_ = PLAYER_JUMP_VELOCITY;
	
	//this->ability_ptr_ = new Ability();
	//this->ability_ptr_ = new DoubleJump();
	this->ability_ptr_ = new Dash();
	
	this->weapon_.id = WEAPON_SWORD;
}

PlayerCharacter::~PlayerCharacter() {

}

AbilityID PlayerCharacter::GetAbilityID() const {
	return this->ability_ptr_->id_;
}

WeaponID PlayerCharacter::GetWeaponID() const {
	//WIP
	return this->weapon_.id;
}

int PlayerCharacter::GetNumOfKeys() const {
	return this->num_of_keys_;
}

void PlayerCharacter::MoveLeft() {
	//Set player looking towards negative x
	this->looking_towards_x_ = -1;

	//If the player already exceeds its top movement speed in that direction
	//we do not do anything as we do not want to overwrite the speed
	if (this->GetVelocityVec().x < -this->move_top_speed_) { return; }

	//Change speed in accordance with facing direction
	this->AlterVelocity();
}

void PlayerCharacter::MoveRight() {
	//Set player as looking towards positive x
	this->looking_towards_x_ = 1;

	//If the player already exceeds its top movement speed in that direction
	//we do not do anything as we do not want to overwrite the speed
	if (this->GetVelocityVec().x > this->move_top_speed_) { return; }

	//Change speed in accordance with facing direction
	this->AlterVelocity();
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
}

void PlayerCharacter::IncreaseKeys() {
	//Increase keys by one
	this->num_of_keys_++;
}