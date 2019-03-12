#include "NPC_boss.h"
#include "../player_character.h"

NPCBoss::NPCBoss(glm::vec3 start_pos) : NPC(
	start_pos,
	OBJECT_ID_BOSS,
	GlobalSettings::Access()->ValueOf("BOSS_HP"),
	GlobalSettings::Access()->ValueOf("BOSS_ATK")
) {
	stage_ = STAGE_1;
	SetScale(8, 8, 1);
	SetOffsets(8, 3);

	laugh1_.LoadSound((char*)"../Resources/Audio/evil_laugh2.wav");
	laugh2_.LoadSound((char*)"../Resources/Audio/evil_laugh3.wav");
	laugh3_.LoadSound((char*)"../Resources/Audio/evil_laugh1.wav");
	
	arm_hit_ground_.LoadSound((char*)"../Resources/Audio/boss_arm_boom.wav");
	laugh1_.PlaySound();

	

	boss_objects_.push_back(new ObjectClass(start_pos, OBJECT_ID_BOSS_HAND));
	boss_objects_[0]->SetPosition(start_pos.x, start_pos.y + 200, 5.0f);
	boss_objects_[0]->SetScale(6, 6, 2);
	boss_objects_[0]->SetOffsets(16,60);
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
			actions_.arm_attack = true; actions_.arm_attack_process = true;
			laugh2_.PlaySound();
			time = 4.0f;
		}
		if (time > 9.0f) {
			time = 0.0f;
			stage_1_counter++;
			if (stage_1_counter >= 4)
			{
				stage_ = STAGE_2;
				time = 0.0f;
				stage_2_counter = 0;
				laugh3_.PlaySound();
			}
		}

	}
	else if (stage_ == STAGE_2) {
		if (time > 3.5f)
		{
			actions_.spawn_mobs = true;
			stage_2_counter++;
			time = 0.0f;
		}
		if (stage_2_counter >= 5)
		{
			stage_ = STAGE_1;
			stage_1_counter = 0;
		}
	}



	ExecuteActions(in_deltatime, in_player_pos);
	UpdateBossObjects(in_deltatime, in_player_pos);
}

void NPCBoss::ExecuteActions(float in_deltatime, glm::vec3 in_player_pos) {
	
	if (actions_.arm_attack) {
		boss_objects_[0]->SetPosition(in_player_pos.x, -900, 5.0f);
		boss_objects_[0]->SetVelocityVec(glm::vec3( 0.0f, -300.0f, 0.0f ));
		//object_pos += in_object_ptr->GetVelocityVec()*in_deltatime;
		actions_.arm_attack = false;
	}
}

void NPCBoss::UpdateBossObjects(float in_deltatime, glm::vec3 in_player_pos) {

	for (int i = 0; i < boss_objects_.size(); i++) {
		glm::vec3 temp_pos = boss_objects_[i]->GetPosition();
		temp_pos += boss_objects_[i]->GetVelocityVec() * in_deltatime;

		if (boss_objects_[i]->GetPosition().y > -1160 || boss_objects_[i]->GetVelocityVec().y > 0) {
			boss_objects_[i]->SetPosition(temp_pos.x, temp_pos.y, temp_pos.z);
		}
		else if (actions_.arm_attack_process)
		{
			actions_.arm_attack_process = false;
			arm_hit_ground_.PlaySound();
			arm_timer = 0;
		}

		if (!actions_.arm_attack_process)
		{
			arm_timer += in_deltatime;
			if (arm_timer > 3)
			{
				boss_objects_[0]->SetVelocityVec(glm::vec3(0.0f, 600.0f, 0.0f));
			}
		}
	}
}

void NPCBoss::UpdateBossDamageToPlayer(float in_deltatime, PlayerCharacter * player_ptr) {

	float knockback = GlobalSettings::Access()->ValueOf("BOSS_KNOCKBACK");
	int boss_hand_damage = GlobalSettings::Access()->ValueOf("BOSS_ATK");

	// check collision 
	for (int i = 0; i < boss_objects_.size(); i++) {
		if (boss_objects_[i]->CheckCollision(player_ptr->GetPoints())) {
			
			player_ptr->TakeDamage(boss_hand_damage);

			//Knockback player
			if (player_ptr->GetPosition().x < boss_objects_[i]->GetPosition().x)
				player_ptr->SetVelocityVec(glm::vec3(-1 * knockback, knockback/8, 0.0f));
			else if (player_ptr->GetPosition().x > boss_objects_[i]->GetPosition().x)
				player_ptr->SetVelocityVec(glm::vec3(knockback, knockback/8, 0.0f));
		}
	}


}

BossActions NPCBoss::GetBossActions() const {
	return actions_;
}

std::vector<ObjectClass*> NPCBoss::GetBossObjectVector() const {
	return boss_objects_;
}
