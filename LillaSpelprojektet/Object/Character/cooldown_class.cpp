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

bool CooldownClass::UpdateCooldown(const float& in_deltatime) {
	//If the cooldown is already 0, return true
	if (this->IsOffCooldown()) { return true; }

	//Other wise decrease the remaining cooldown with the given amount
	this->remaining_cooldown_ -= in_deltatime;

	//Ensure value isn't below 0
	if (this->remaining_cooldown_ <= 0.0f) {
		this->remaining_cooldown_ = 0.0f;
		//Now that it is 0 return true
		return true;
	}
	//Otherwise return false
	return false;
}

bool CooldownClass::IsOffCooldown() {
	return (this->remaining_cooldown_ == 0.0f);
}