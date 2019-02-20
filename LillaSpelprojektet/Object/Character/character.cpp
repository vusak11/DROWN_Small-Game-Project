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
	this->max_health_;
}

int Character::GetCurrentHealth() const {
	this->current_health_;
}

int Character::GetAttackPower() const {
	this->attack_power_;
}

int Character::TakeDamage(int in_dmg) {
	//If the in parameter is negative, return false
	if (in_dmg< 0) {
		throw std::invalid_argument(
			"ERROR::CHARACTER::HEALDAMAGE::Damage may not be negative"
		);
	}

	this->current_health_ -= in_dmg;

	if (this->current_health_ <= 0) {
		this->current_health_ =0;
		return 1;
	}

	return 0;
}

int Character::HealDamage(int in_heal) {
	//If the in parameter is negative, return false
	if (in_heal < 0) {
		throw std::invalid_argument(
			"ERROR::CHARACTER::HEALDAMAGE::Heal may not be negative"
		);
	}

	this->current_health_ += in_heal;

	if (this->current_health_ >= this->max_health_) {
		this->current_health_ = this->max_health_;
		return 1;
	}

	return 0;
}
