#ifndef DROP_H
#define DROP_H

#include "../object_class.h"
#include "../Character/player_character.h"

class Drop : public ObjectClass {
private:

public:
	Drop();
	~Drop();

	virtual bool CheckCollision(const BoxPoints& in_box);
	//virtual bool TriggerEvent(PlayerCharacter& in_player);

};

#endif // !DROP_H
