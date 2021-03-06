#ifndef NPC_BOSS_H
#define NPC_BOSS_H

#include "../npc.h"

class PlayerCharacter;

struct BossActions {		//A way for the object handler to see what actions to make for the boss npc
	bool arm_attack = false;
	bool arm_attack_process = false;
	bool spawn_mobs = false;
	bool spawn_ghost = false;
	bool spawn_jombo = false;
	bool attack_light = false;

};

enum BossStage {
	STAGE_1,
	STAGE_2,
	STAGE_3,
	STAGE_4
};

class NPCBoss : public NPC {
private:
	BossStage stage_;
	
	SoundUnit laugh1_;
	SoundUnit laugh2_;
	SoundUnit laugh3_;
	SoundUnit damaged_;
	SoundUnit phase_3_sound_;
	SoundUnit arm_hit_ground_;
	SoundUnit defeat_sound_;
	std::vector<ObjectClass*> boss_objects_;

	int health_last_frame_;

	float time_ = -5.0f;
	float phases_complete_ = 0; // add one after each phace complete to scale the boss.
	float light_timer_ = 0;

	// stage 1 variables
	int stage_1_counter_ = 0;
	float arm_timer_ = 0;

	// stage 2 variables
	int stage_2_counter = 0;



public:
	NPCBoss(glm::vec3 start_pos);
	~NPCBoss();

	BossActions actions_;

	void ExecuteAI(float in_deltatime, glm::vec3 in_player_pos);
	void ExecuteActions(float in_deltatime, glm::vec3 in_player_pos);	// Execute actions on the boss side of the code. (move its objects etc.)

	void UpdateBossObjects(float in_deltatime, glm::vec3 in_player_pos);	// This replaces the physics engine pass for all the other objects.
	void UpdateBossDamageToPlayer(float in_deltatime, PlayerCharacter* player_ptr);


	BossActions GetBossActions() const;
	BossStage GetBossStage() const;
	std::vector<ObjectClass*> GetBossObjectVector() const;
	bool IsStage4Complete();

};


#endif // !NPC_BOSS_H
