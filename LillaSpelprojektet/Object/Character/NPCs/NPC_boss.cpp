#include "NPC_boss.h"

NPCBoss::NPCBoss(glm::vec3 start_pos) : NPC(
	start_pos,
	OBJECT_ID_BOSS,
	GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_HP"),
	GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_ATK")
) {
	stage_ = STAGE_1;
	SetScale(8, 8, 1);

	laugh1_.LoadSound((char*)"../Resources/Audio/evil_laugh2.wav");
	laugh2_.LoadSound((char*)"../Resources/Audio/evil_laugh3.wav");
	laugh1_.PlaySound();

	boss_objects_.push_back(new ObjectClass(start_pos, OBJECT_ID_BOSS_HAND));
	boss_objects_[0]->SetPosition(start_pos.x, start_pos.y + 200, 5.0f);
	boss_objects_[0]->SetScale(6, 6, 1);
}

NPCBoss::~NPCBoss() {
}

void NPCBoss::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos) {
	//SetPosition(160,-1100,10);

	// Set to -5 to delay the start
	static float time = -5.0f;
	time += in_deltatime;

	
	// set time to 0 after each cycle
	if (stage_ == STAGE_1) {

		if (time > 0.0f && time < 3.0f) {
			actions_.arm_attack = true;
			laugh2_.PlaySound();
			time = 4.0f;
		}
		if (time > 10.0f) {
			time = 0.0f;
		}

	}
	else if (stage_ == STAGE_2) {
		
	}



	ExecuteActions(in_deltatime, in_player_pos);
	UpdateBossObjects(in_deltatime, in_player_pos);
}

void NPCBoss::ExecuteActions(float in_deltatime, glm::vec3 in_player_pos) {
	
	if (actions_.arm_attack) {
		boss_objects_[0]->SetPosition(in_player_pos.x, -900, 5.0f);
		boss_objects_[0]->SetVelocityVec(glm::vec3( 0.0f, -280.0f, 0.0f ));
		//object_pos += in_object_ptr->GetVelocityVec()*in_deltatime;
		actions_.arm_attack = false;
	}
}

void NPCBoss::UpdateBossObjects(float in_deltatime, glm::vec3 in_player_pos) {

	for (int i = 0; i < boss_objects_.size(); i++) {
		glm::vec3 temp_pos = boss_objects_[i]->GetPosition();
		temp_pos += boss_objects_[i]->GetVelocityVec() * in_deltatime;

		//boss_objects_[i]->SetPosition(temp_pos.x, temp_pos.y, temp_pos.z);

		if (boss_objects_[i]->GetPosition().y > -1160) {
			boss_objects_[i]->SetPosition(temp_pos.x, temp_pos.y, temp_pos.z);
		}
	}
}

BossActions NPCBoss::GetBossActions() const {
	return actions_;
}

std::vector<ObjectClass*> NPCBoss::GetBossObjectVector() const {
	return boss_objects_;
}
