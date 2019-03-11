#ifndef NPC_BOSS_H
#define NPC_BOSS_H

#include "../npc.h"

struct BossActions {		//A way for the object handler to see what actions to make for the boss npc
	bool arm_attack = false;
	bool spawn_mobs = false;
};

enum BossStage {
	STAGE_1,
	STAGE_2
};

class NPCBoss : public NPC {
private:
	BossStage stage_;
	BossActions actions_;
	SoundUnit laugh1_;
	SoundUnit laugh2_;
	std::vector<ObjectClass*> boss_objects_;
public:
	NPCBoss(glm::vec3 start_pos);
	~NPCBoss();

	void ExecuteAI(float in_deltatime, glm::vec3 in_player_pos);
	void ExecuteActions(float in_deltatime, glm::vec3 in_player_pos);	// Execute actions on the boss side of the code. (move its objects etc.)

	void UpdateBossObjects(float in_deltatime, glm::vec3 in_player_pos);	// This replaces the physics engine pass for all the other objects.

	BossActions GetBossActions() const;
	std::vector<ObjectClass*> GetBossObjectVector() const;

};


#endif // !NPC_BOSS_H
