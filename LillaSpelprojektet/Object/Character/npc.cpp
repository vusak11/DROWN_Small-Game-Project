#include "npc.h"

//Private--------------------------------------------------

//Public---------------------------------------------------
NPC::NPC(glm::vec3 start_pos, ObjectID id) : Character(start_pos, id, NPC_START_HP, NPC_START_ATK) {
	state_ = NPC_STATE_IDLE;
}

NPC::~NPC() {
}

void NPC::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos) {

}