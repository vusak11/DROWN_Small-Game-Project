#ifndef NPC_H
#define NPC_H

#include "character.h"

enum NPC_STATE {
	NPC_STATE_IDLE,		//0: 
	NPC_STATE_AGGRO		//1: 
};



class NPC : public Character {
protected:
	NPC_STATE state_;
private:
	
public:
	NPC(glm::vec3 start_pos, ObjectID id, int start_hp, int start_atk);
	~NPC();

	virtual void ExecuteAI(float in_deltatime, glm::vec3 in_player_pos);
};


#endif // !NPC_H
