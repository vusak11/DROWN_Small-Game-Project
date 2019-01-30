#include "character.h"

Character::Character(){
}

Character::~Character(){
}

void Character::Render(){
}

void Character::SetPosition(glm_vec4 const pos){
	mPosition_.m128_f32[0] = pos.m128_f32[0];
	mPosition_.m128_f32[1] = pos.m128_f32[1];
	mPosition_.m128_f32[2] = pos.m128_f32[2];
	mPosition_.m128_f32[3] = pos.m128_f32[3];
}

void Character::SetHealth(int const health){
	mHealth_ = health;
}

void Character::SetPower(int const power){
	mPower_ = power;
}

glm_vec4 Character::GetPosition()
{
	return mPosition_;
}

int Character::GetHealth(){
	return mHealth_;
}

int Character::GetPower(){
	return mPower_;
}
