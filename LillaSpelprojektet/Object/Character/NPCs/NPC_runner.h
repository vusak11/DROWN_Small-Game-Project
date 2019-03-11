#ifndef NPC_RUNNER_H
#define NPC_RUNNER_H

#include "../npc.h"

class NPCRunner : public NPC {
private:

	float time_to_next_move_;
	int next_move_index_;		// 0 = idle, 1 = move right, 2 = move left
	int aggro_range_;
	glm::vec2 knockback_;

public:
	NPCRunner(glm::vec3 start_pos, ObjectID id);
	~NPCRunner();

	void ExecuteAI(float in_deltatime, glm::vec3 in_player_pos);
	bool Attack(Character& in_target);
};


#endif // !NPC_RUNNER_H
