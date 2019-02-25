#ifndef NPC_H
#define NPC_H

#include "character.h"

class NPC : public Character {
private:

	float TEMP_CD_ = 0.0f;
	float TEMP_ROT = 0.0;

public:
	NPC(glm::vec3 start_pos, ObjectID id = OBJECT_ID_DUMMY);
	~NPC();

	virtual void ExecuteAI(float in_deltatime/*, glm::vec3 in_player_pos*/);
};


#endif // !NPC_H
