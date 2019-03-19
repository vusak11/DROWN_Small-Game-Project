#include "npc.h"

//Private--------------------------------------------------

//Public---------------------------------------------------
NPC::NPC(glm::vec3 start_pos, ObjectID id, int start_hp, int start_atk)
	: Character(start_pos, id, start_hp, start_atk) {
	state_ = NPC_STATE_IDLE;
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