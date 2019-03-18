#ifndef NPC_H
#define NPC_H

#include "character.h"

enum NPC_STATE {
	NPC_STATE_IDLE,		//0: 
	NPC_STATE_AGGRO		//1: 
};



class NPC : public Character {
private:
	bool dead_;
	float death_time_;
	float shrink_rate_;

protected:
	NPC_STATE state_;

public:
	NPC(glm::vec3 start_pos, ObjectID id, int start_hp, int start_atk);
	~NPC();

	virtual bool Attack(Character& in_target);
	virtual void ExecuteAI(float in_deltatime, glm::vec3 in_player_pos);

	bool ExecuteDeath(const float& in_deltatime);
	bool IsDead();
};
#endif // !NPC_H
