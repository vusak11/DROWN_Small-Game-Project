#include "character.h"

Character::Character(){
}

Character::~Character(){
}

void Character::SetHealth(int const health){
	health_ = health;
}

void Character::SetPower(int const power){
	power_ = power;
}

int Character::GetHealth(){
	return health_;
}

int Character::GetPower(){
	return power_;
}
