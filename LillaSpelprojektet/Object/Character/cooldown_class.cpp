#include "cooldown_class.h"

//Public---------------------------------------------------
CooldownClass::CooldownClass(float in_total_cooldown) {
	this->total_cooldown_ = in_total_cooldown;
	this->remaining_cooldown_ = 0.0f;
}

CooldownClass::~CooldownClass() {

}

void CooldownClass::BeginCooldown() {
	this->remaining_cooldown_ = this->total_cooldown_;
}

void CooldownClass::ResetCooldown() {
	this->remaining_cooldown_ = 0.0f;
}

bool CooldownClass::UpdateCooldown(float in_deltatime) {
	
	//NTS: There might be a smarter order to do these checks

	//Decrease the remaining cooldown with the given amount
	this->remaining_cooldown_ -= in_deltatime;

	//If the remaining cooldown is 0 or lower
	if (this->remaining_cooldown_ <= 0.0f) {
		//Set it to 0 and return true
		this->remaining_cooldown_ = 0.0f;
		return true;
	}

	return false;
}