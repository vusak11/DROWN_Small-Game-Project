#ifndef NPC_BOSS_H
#define NPC_BOSS_H

#include "../npc.h"


enum BossStage {
	STAGE_1,
	STAGE_2
};

class NPCBoss : public NPC {
private:
	BossStage stage_;
	SoundUnit test_sound;
public:
	NPCBoss(glm::vec3 start_pos);
	~NPCBoss();

	void ExecuteAI(float in_deltatime, glm::vec3 in_player_pos);

};


#endif // !NPC_BOSS_H
