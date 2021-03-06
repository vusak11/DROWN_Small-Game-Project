#include "character.h"

Character::Character(glm::vec3 start_pos, ObjectID id, int start_hp, int start_atk)
	: ObjectClass(start_pos, id){

	this->max_health_ = start_hp;
	this->current_health_ = this->max_health_;

	this->attack_power_ = start_atk;

}

Character::~Character(){
}

void Character::SetMaxHealth(int in_hp) {
	if (in_hp < 1) {
		//If value is negative do nothing :D
		return;
	}
	this->max_health_ = in_hp;
}

void Character::SetCurrentHealth(int in_hp) {

	if (in_hp < 0) {
		//If value is negative do nothing :D
		return;
	}
	else if (in_hp > this->max_health_) {
		//If value is higher than max, set it to max
		in_hp = this->max_health_;
	}

	this->current_health_ = in_hp;
}

void Character::SetAttackPower(int in_atk) {
	if (in_atk < 1) {
		//If value is less than 1 do nothing :D
		return;
	}
	this->attack_power_ = in_atk;
}

int Character::GetMaxHealth() const {
	return this->max_health_;
}

int Character::GetCurrentHealth() const {
	return this->current_health_;
}

int Character::GetAttackPower() const {
	return this->attack_power_;
}

int Character::GetFacingDirection() const {
	return this->looking_towards_x_;
}

int Character::TakeDamage(int in_dmg) {
	//Returns	-1 if in-value is negative (operation fails)
	//			 0 if operation was successful
	//			 1 if cap (max hp/0 hp) was hit

	//If the in parameter is negative, return -1
	if (in_dmg < 0) {
		return -1;
	}

	this->current_health_ -= in_dmg;

	if (this->current_health_ <= 0) {
		this->current_health_ =0;
		return 1;
	}

	return 0;
}

int Character::HealDamage(int in_heal) {
	//If the in parameter is negative, return -1
	if (in_heal < 0) {
		return -1;
	}

	this->current_health_ += in_heal;

	if (this->current_health_ >= this->max_health_) {
		this->current_health_ = this->max_health_;
		return 1;
	}

	return 0;
}

bool Character::IncreaseMaxHealth(int in_hp) {
	//If the in parameter is negative, return false
	if (in_hp < 0) {
		return false;
	}

	//Otherwise increase stat and return true
	this->max_health_ += in_hp;

	return true;
}

bool Character::IncreaseAttack(int in_atk) {
	//If the in parameter is negative, return false
	if (in_atk < 0) {
		return false;
	}

	//Otherwise increase stat and return true
	this->attack_power_ += in_atk;

	return true;
}

bool Character::IncreaseSpeed(int in_spd) {
	//If the in parameter is negative, return false
	if (in_spd < 0) {
		return false;
	}

	//Otherwise increase stat and return true
	this->move_top_speed_ += in_spd;
	this->move_acceleration_ = this->move_top_speed_ / this->move_acceleration_rate_;

	return true;
}

void Character::TurnLeft(const float& in_deltatime) {
	//Turn the model leftwards (negative direction) with adjustment for deltatime
	float turn_radians = this->turn_rate_radians_*in_deltatime;
	float new_rotation = this->rotation_around_y_ - turn_radians;

	//If the new orientation is further than -PI/2 snap it to -PI/2
	if (new_rotation < glm::radians(-90.0f)) { new_rotation = glm::radians(-90.0f); }

	//std::cout << "Rot L: " << glm::degrees(new_rotation) << std::endl;

	this->SetRotation(new_rotation);
}

void Character::TurnRight(const float& in_deltatime) {
	//Turn the model rightwards (positive direction) with adjustment for deltatime
	float turn_radians = this->turn_rate_radians_*in_deltatime;
	float new_rotation = this->rotation_around_y_ + turn_radians;

	//If the new orientation is further than PI/2 snap it to PI/2
	if (new_rotation > glm::radians(90.0f)) { new_rotation = glm::radians(90.0f); }

	//std::cout << "Rot R: " << glm::degrees(new_rotation) << std::endl;

	this->SetRotation(new_rotation);
}


