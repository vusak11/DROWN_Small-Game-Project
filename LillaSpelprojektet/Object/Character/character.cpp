#include "character.h"

Character::Character(glm::vec3 start_pos, ObjectID id) : ObjectClass(start_pos, id){
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
