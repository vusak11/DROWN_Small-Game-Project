#ifndef NPC_RUNNER_H
#define NPC_RUNNER_H

#include "../npc.h"

class NPCRunner : public NPC {
private:

	float time_to_next_move_;
	int next_move_index_;		// 0 = idle, 1 = move right, 2 = move left
	float aggro_range_;
	glm::vec2 knockback_;
	int health_last_frame_;	// used for calculating stun
	bool stunned_ = false;
	float stun_timer_ = 0;

	float jump_velocity_;
	float aggro_speed_;

public:
	NPCRunner(glm::vec3 start_pos, ObjectID id);
	~NPCRunner();

	void ExecuteAI(float in_deltatime, glm::vec3 in_player_pos);
	bool Attack(Character& in_target);

	void SetAggroRange(int aggro_range);
	void SetJumpVelocity(float jump_velocity);
	void SetAggroVelocity(float aggro_velocity);
};


#endif // !NPC_RUNNER_H
