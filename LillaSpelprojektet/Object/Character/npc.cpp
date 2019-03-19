#include "npc.h"

//Private--------------------------------------------------

//Public---------------------------------------------------
NPC::NPC(glm::vec3 start_pos, ObjectID id, int start_hp, int start_atk)
	: Character(start_pos, id, start_hp, start_atk) {
	this->state_ = NPC_STATE_IDLE;

	this->is_dead_ = false;
	this->death_time_ = 0.5f;
}

NPC::~NPC() {
}

bool NPC::Attack(Character& in_target) {
	if (in_target.GetCurrentHealth() <= 0) {
		in_target.SetCurrentHealth(in_target.GetCurrentHealth() - 1);
		return true;
	}
	return false;
}

void NPC::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos) {
	
}

bool NPC::ExecuteDeath(const float& in_deltatime) {
	
	//Don't do this if we have more than 0 health
	if (this->GetCurrentHealth() > 0) { return false; }

	//Otherwise set our hitbox to huge negatives
	this->SetOffsets(-10000.0f, -10000.0f);
	//Stop movement
	this->SetVelocityVec(glm::vec3(0.0f, 10.0f, 0.0f));
	//And disable physics
	this->SetUsingPhysics(false);

	//Then start shrinking our model
	glm::vec3 new_scale = this->GetScale();

	new_scale.x -= new_scale.x * in_deltatime;
	new_scale.y -= new_scale.y * in_deltatime;
	new_scale.z -= new_scale.z * in_deltatime;

	this->SetScale(new_scale.x, new_scale.y, new_scale.z);

	//Count for how long we have shrunk
	this->death_time_ -= in_deltatime;

	//When we are done dying, be dead :D
	if (death_time_ <= 0.0f) { this->is_dead_ = true; }

	//Since we are dyiong or dead return true
	return true;
}

bool NPC::IsDead() {
	return this->is_dead_;
}