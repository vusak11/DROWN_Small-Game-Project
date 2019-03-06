#include "NPC_boss.h"

NPCBoss::NPCBoss(glm::vec3 start_pos) : NPC(
	start_pos,
	OBJECT_ID_DUMMY,
	GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_HP"),
	GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_ATK")
) {
	stage_ = STAGE_1;
	//test_sound.LoadSound((char*)"../../../../Resources/Audio/");
}

NPCBoss::~NPCBoss() {
}

void NPCBoss::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos) {

}
