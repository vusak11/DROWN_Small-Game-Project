#ifndef NPC_H
#define NPC_H

#include "character.h"

class NPC : public Character {
private:

public:
	NPC(glm::vec3 start_pos, ObjectID id = OBJECT_ID_DUMMY);
	~NPC();

	virtual void ExecuteAI(glm::vec3 in_player_pos);
};


#endif // !NPC_H
