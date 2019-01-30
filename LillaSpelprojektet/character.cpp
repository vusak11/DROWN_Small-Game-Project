#include "character.h"

Character::Character(){
}

Character::~Character(){
}

void Character::Render(){
}

void Character::SetPosition(glm_vec4 const pos){
	position_.m128_f32[0] = pos.m128_f32[0];
	position_.m128_f32[1] = pos.m128_f32[1];
	position_.m128_f32[2] = pos.m128_f32[2];
	position_.m128_f32[3] = pos.m128_f32[3];
}

void Character::SetHealth(int const health){
	health_ = health;
}

void Character::SetPower(int const power){
	power_ = power;
}

glm_vec4 Character::GetPosition()
{
	return position_;
}

int Character::GetHealth(){
	return health_;
}

int Character::GetPower(){
	return power_;
}
