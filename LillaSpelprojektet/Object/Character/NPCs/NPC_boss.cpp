#include "NPC_boss.h"
#include "../player_character.h"
//#include "NPC_ghost.h"

NPCBoss::NPCBoss(glm::vec3 start_pos) : NPC(
	start_pos,
	OBJECT_ID_BOSS,
	GlobalSettings::Access()->ValueOf("BOSS_HP"),
	GlobalSettings::Access()->ValueOf("BOSS_ATK")
) {
	stage_ = STAGE_1;
	SetScale(12, 12, 4);
	SetOffsets(40, 12);

	health_last_frame_ = GetCurrentHealth();

	laugh1_.LoadSound((char*)"../Resources/Audio/evil_laugh2.wav");
	laugh2_.LoadSound((char*)"../Resources/Audio/evil_laugh3.wav");
	laugh3_.LoadSound((char*)"../Resources/Audio/evil_laugh1.wav");
	damaged_.LoadSound((char*)"../Resources/Audio/boss_damaged.wav");
	phase_3_sound_.LoadSound((char*)"../Resources/Audio/boss_stage_3.wav");
	
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
	static float time_stage3 = 0.0f;
	time += in_deltatime;
	time_stage3 += in_deltatime;
	light_timer_ += in_deltatime;

	int health = GetCurrentHealth();
	if (health_last_frame_ > health)
	{
		damaged_.PlaySound();
		std::cout << "boss health: " << GetCurrentHealth() << std::endl;
	}
	health_last_frame_ = health;

	//UpdateBossNPCs(in_deltatime, in_player_pos);

	// set time to 0 after each cycle
	if (stage_ == STAGE_1) {
		
		if (time > 0.0f && time < 3.0f) {
			actions_.arm_attack = true; actions_.arm_attack_process = true;
			laugh2_.PlaySound();
			actions_.attack_light = true;
			light_timer_ = 0;
			time = 4.0f;
			
			if (phases_complete_ > 3)
			{
				actions_.spawn_ghost = true;
			}
			if (phases_complete_ > 9)
			{
				actions_.spawn_mobs = true;
			}
			
		}
		if (time > 9.0f) {
			time = 0.0f;
			stage_1_counter++;
			if (stage_1_counter >= 5)
			{
				stage_ = STAGE_2;
				time = 0.0f;
				stage_2_counter = 0;
				laugh3_.PlaySound();
				SetVelocityVec(glm::vec3(0.0f, +30.0f, 0.0f));
				phases_complete_++;
				if (phases_complete_ > 2)
				{
					actions_.spawn_ghost = true;
				}
			}
		}
		if (health < GetMaxHealth() * 0.8f)
		{
			stage_ = STAGE_3;
			stage_1_counter = 0;
			time = 0.0f;
			time_stage3 = 1.0f;
			phase_3_sound_.PlaySound();
		}
		if (GetPosition().y > -1110)
		{
			SetVelocityVec(glm::vec3(0.0f, -30.0f, 0.0f));
		}
	}
	else if (stage_ == STAGE_2) {
		if (GetPosition().y < -900)
		{
			SetVelocityVec(glm::vec3(0.0f, +30.0f, 0.0f));
		}
		if (time > __max(3.0f - phases_complete_ * 0.3f, 0.3f) )
		{
			actions_.spawn_mobs = true;
			stage_2_counter++;
			time = 0.0f;
			if (phases_complete_ > 6)
			{
				actions_.spawn_ghost = true;
			}
		}
		int nr_of_iterations = rand() % 5 + 2;
		if (stage_2_counter >= nr_of_iterations)
		{
			stage_ = STAGE_1;
			stage_1_counter = 0;
			time = 7.5f;
			SetVelocityVec(glm::vec3(0.0f, -90.0f, 0.0f));
			glm::vec3 temp_pos = GetPosition();
			SetPosition(160, temp_pos.y, temp_pos.z);
			phases_complete_++;
			if (phases_complete_ > 1)
			{
				actions_.spawn_ghost = true;
			}
		}

	}
	else if (stage_ == STAGE_3)
	{
		if (time > 7) {
			actions_.arm_attack = true; actions_.arm_attack_process = true;
			actions_.spawn_ghost = true;
			laugh2_.PlaySound();
			actions_.attack_light = true;
			light_timer_ = 0;
			time = 0;
		}

		if (time_stage3 > 2.0f && time_stage3 < 3.0f) {
			actions_.spawn_mobs = true;
			actions_.spawn_ghost = true;
			time_stage3 = 3.01f;
		}

		if (time_stage3 > 6.0f && time_stage3 < 7.0f) {
			actions_.spawn_mobs = true;
			actions_.spawn_ghost = true;
			time_stage3 = 7.01f;
		}

		if (time_stage3 > 10.0f && time_stage3 < 12.0f) {
			actions_.spawn_jombo = true;
			actions_.spawn_ghost = true;
			time_stage3 = 0.00f;
		}
		
		if (GetPosition().y > -1110)
		{
			SetVelocityVec(glm::vec3(0.0f, -30.0f, 0.0f));
		}
	}
	else if (stage_ == STAGE_4)
	{
		SetVelocityVec(glm::vec3(0.0f, -time * 9.87f, 0.0f));
	}

	if (light_timer_ > 1.5f)
	{
		actions_.attack_light = false;
	}

	// Trigger when boss is defeated
	if (current_health_ <= 0)
	{
		time = 0;
		STAGE_4;
	}

	ExecuteActions(in_deltatime, in_player_pos);
	UpdateBossObjects(in_deltatime, in_player_pos);
}

void NPCBoss::ExecuteActions(float in_deltatime, glm::vec3 in_player_pos) {
	
	// Set x velocity depending on player position
	glm::vec3 temp_velocity = GetVelocityVec();
	if (in_player_pos.y > -1170)
	{
		// this acts strange with negative values obv
		float diff = (GetPosition().x - in_player_pos.x);
		float move_speed = __max(60 - 4.0 * phases_complete_, 15);
		if (diff < 0)
		{
			SetVelocityVec({ -1 * move_speed, temp_velocity.y , temp_velocity.z });
		}
		else if (diff > 0)
		{
			SetVelocityVec({ move_speed, temp_velocity.y, temp_velocity.z });
		}
	}

	if (actions_.arm_attack) {
		boss_objects_[0]->SetPosition(in_player_pos.x, -900, 5.0f);
		boss_objects_[0]->SetVelocityVec(glm::vec3( 0.0f, -300.0f, 0.0f ));
		//object_pos += in_object_ptr->GetVelocityVec()*in_deltatime;
		actions_.arm_attack = false;
	}
}

void NPCBoss::UpdateBossObjects(float in_deltatime, glm::vec3 in_player_pos) {
	
	glm::vec3 temp_pos = GetPosition();
	glm::vec3 temp_velocity = GetVelocityVec();
	temp_pos += GetVelocityVec() * in_deltatime;

	if (temp_velocity.x > 0)
	{
		if (GetPosition().x < 220) {
			SetPosition(temp_pos.x, GetPosition().y, temp_pos.z);
		}
	}
	else if (temp_velocity.x < 0)
	{
		if (GetPosition().x > 100) {
			SetPosition(temp_pos.x, GetPosition().y, temp_pos.z);
		}
	}

	if (temp_velocity.y != 0)
	{
		if (temp_velocity.y > 0 && GetPosition().y < -900)
		{
			SetPosition(GetPosition().x, temp_pos.y, GetPosition().z);
		}
		else if (temp_velocity.y < 0 && GetPosition().y > -1110)
		{
			SetPosition(GetPosition().x, temp_pos.y, GetPosition().z);
		}
		else
		{
			SetVelocityVec(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
	


	// Boss objects
	for (int i = 0; i < boss_objects_.size(); i++) {
		temp_pos = boss_objects_[i]->GetPosition();
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

//void NPCBoss::UpdateBossNPCs(float in_deltatime, glm::vec3 in_player_pos) {
//
//	/*for (size_t i = 0; i < boss_NPCs_.size(); i++)
//	{
//		if (boss_NPCs_[i]->GetHealthLastFrame() > boss_NPCs_[i]->GetCurrentHealth())
//		{
//			damaged_.PlaySound();
//			std::cout << "Killed ghost: " << std::endl;
//			boss_NPCs_.erase(boss_NPCs_.begin() + i);
//			i--;
//		}
//	}*/
//}

BossActions NPCBoss::GetBossActions() const {
	return actions_;
}

BossStage NPCBoss::GetBossStage() const
{
	return stage_;
}

std::vector<ObjectClass*> NPCBoss::GetBossObjectVector() const {
	return boss_objects_;
}

//std::vector<NPCGhost*> NPCBoss::GetBossNPCVector() const
//{
//	return boss_NPCs_;
//}
