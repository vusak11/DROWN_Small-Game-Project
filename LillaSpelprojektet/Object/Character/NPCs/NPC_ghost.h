#ifndef NPC_GHOST_H
#define NPC_GHOST_H

#include "../npc.h"

class NPCGhost : public NPC {
private:

	float time_to_next_move_;
	int next_move_index_;		// 0 = idle, 1 = move right, 2 = move left
	int aggro_range_;
	glm::vec2 knockback_;
	int health_last_frame_;	// used for calculating stun
	bool stunned_ = false;
	float stun_timer = 0;

public:
	NPCGhost(glm::vec3 start_pos, ObjectID id);
	~NPCGhost();

	void ExecuteAI(float in_deltatime, glm::vec3 in_player_pos);
	bool Attack(Character& in_target);

	int GetHealthLastFrame() const;

	void SetAggroRange(int aggro_range);
};


#endif // !NPC_GHOST_H
