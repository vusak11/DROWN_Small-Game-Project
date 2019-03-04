#include "character.h"

Character::Character(glm::vec3 start_pos, ObjectID id) : ObjectClass(start_pos, id){

	//Set stats using ID

}

Character::~Character(){
}

void Character::SetMaxHealth(int in_hp) {
	if (in_hp < 1) {
		throw std::invalid_argument(
			"ERROR::CHARACTER::SETMAXHEALTH::Max health cannot be lower than 1"
		);
	}
	this->max_health_ = in_hp;
}

void Character::SetCurrentHealth(int in_hp) {
	if (in_hp < 0	||	in_hp > this->max_health_) {
		throw std::invalid_argument(
			"ERROR::CHARACTER::SETCURRENTHEALTH::Argument out of valid scope"
		);
	}
	this->current_health_ = in_hp;
}

void Character::SetAttackPower(int in_atk) {
	if (in_atk < 1) {
		throw std::invalid_argument(
			"ERROR::CHARACTER::SETATTACKPOWER::Attack power cannot be lower than 1"
		);
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

int Character::TakeDamage(int in_dmg) {
	//If the in parameter is negative, return -1
	if (in_dmg < 0) {
		//throw std::invalid_argument(
		//	"ERROR::CHARACTER::TAKEDAMAGE::Damage may not be negative"
		//);
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
		//throw std::invalid_argument(
		//	"ERROR::CHARACTER::HEALDAMAGE::Heal may not be negative"
		//);
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
	this->current_health_ += in_hp;

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

bool Character::SetKeyStatus(bool in_key) {
	//If the in parameter is positive, set has_key variable to true
	if (in_key) {
		this->has_key_ = true;
	}
	return this->has_key_;
}


void Character::TurnLeft(const float& in_deltatime) {
	//Turn the model leftwards (negative direction) with adjustment for deltatime
	float turn_radians = glm::radians((float)OBJECT_TURN_RATE)*in_deltatime;
	float new_rotation = this->rotation_around_y_ - turn_radians;

	//If the new orientation is further than -PI/2 snap it to -PI/2
	if (new_rotation < glm::radians(-90.0f)) { new_rotation = glm::radians(-90.0f); }

	//std::cout << "Rot L: " << glm::degrees(new_rotation) << std::endl;

	this->SetRotation(this->rotation_around_x_, new_rotation, this->rotation_around_z_);
}

void Character::TurnRight(const float& in_deltatime) {
	//Turn the model rightwards (positive direction) with adjustment for deltatime
	float turn_radians = glm::radians((float)OBJECT_TURN_RATE)*in_deltatime;
	float new_rotation = this->rotation_around_y_ + turn_radians;

	//If the new orientation is further than PI/2 snap it to PI/2
	if (new_rotation > glm::radians(90.0f)) { new_rotation = glm::radians(90.0f); }

	//std::cout << "Rot R: " << glm::degrees(new_rotation) << std::endl;

	this->SetRotation(this->rotation_around_x_, new_rotation, this->rotation_around_z_);
}
