#include "ability.h"
#include "player_character.h"	//Follow through on forward declaraction

//#include <iostream>			//TEMP

Ability::Ability() {
	this->id_ = ABILITY_NONE;
}

Ability::~Ability() {}

bool Ability::ExecuteAbility(PlayerCharacter& in_player) {
	//This is the base ability that does nothing
	return true;
}
//---------------------------------------------------------
DoubleJump::DoubleJump() {
	this->id_ = ABILITY_DOUBLE_JUMP;
	this->available_ = true;
}

DoubleJump::~DoubleJump() {}

bool DoubleJump::ExecuteAbility(PlayerCharacter& in_player) {

	//If the double jump is not available, return false
	if (!this->available_) { return false; }

	//Otherwise, set player velocity towards positive y
	//to half their normal jump speed
	glm::vec3 new_velocity = in_player.GetVelocityVec();
	new_velocity.y = in_player.jump_speed_ / 2;
	in_player.SetVelocityVec(new_velocity);

	//Then set double jump to unavailable
	//NTS: It becomes available again in the landing function
	this->available_ = false;

	//Ability use successful, return true
	return true;
}
//---------------------------------------------------------
Dash::Dash() : CooldownClass(GlobalSettings::Access()->ValueOf("DASH_COOLDOWN")) {
	this->id_ = ABILITY_DASH;
	this->dash_velocity_ = GlobalSettings::Access()->ValueOf("DASH_VELOCITY");
}

Dash::~Dash(){}

bool Dash::ExecuteAbility(PlayerCharacter& in_player) {
	
	//Check if the dash is off cooldown, otherwise return false
	if (!this->IsOffCooldown()) { return false; }

	//Get only the x-axis velocity (will give us direction of movement)
	glm::vec3 new_velocity = glm::vec3(in_player.GetVelocityVec().x, 0.0f, 0.0f);

	if (new_velocity.x == 0.0f) { return false; }

	//Scale it in accordance with the dash velocity
	new_velocity = glm::normalize(new_velocity) * this->dash_velocity_;


	//Set it as new player velocity
	in_player.SetVelocityVec(new_velocity);

	//Set the ability on cooldown
	this->BeginCooldown();

	return true;
}
//---------------------------------------------------------