#include "npc.h"

//Private--------------------------------------------------

//Public---------------------------------------------------
NPC::NPC(glm::vec3 start_pos, ObjectID id) : Character(start_pos, id) {
	
}

NPC::~NPC() {

}

void NPC::ExecuteAI(float in_deltatime/*, glm::vec3 in_player_pos*/) {

	//TEMP: Does a thing every 3 secs
	this->TEMP_CD_ -= in_deltatime;
	if (this->TEMP_CD_ <= 0.0f) {
		//this->SetVelocityVec(glm::vec3(0.0f, 50.0f, 0.0f));
		this->TEMP_ROT += 3;
		this->SetRotation(0.0f, this->TEMP_ROT, 0.0f);
		this->TEMP_CD_ = 3.0;
	}
	//TEMP


}