#ifndef DROP_H
#define DROP_H

#include "../object_class.h"
#include "../Character/player_character.h"
#include "../../Globals.h"
//#include "../../global_settings.h"

//---------------------------------------------------------
//-----------------ABSTRACT-BASE-CLASS---------------------
//---------------------------------------------------------

class Drop : public ObjectClass {
private:
	virtual bool TriggerEvent(PlayerCharacter& in_player) = 0;

public:
	Drop();
	~Drop();

	bool CheckCollision(PlayerCharacter& in_player);
};

//---------------------------------------------------------
//--------------------DERIVED-CLASSES----------------------
//---------------------------------------------------------

class HPRestoreDrop : public Drop {
private:
	int health_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	HPRestoreDrop();
	~HPRestoreDrop();
};

//---------------------------------------------------------

class HPUpDrop : public Drop {
private:
	int health_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	HPUpDrop();
	~HPUpDrop();
};

//---------------------------------------------------------

class AtkUpDrop : public Drop {
private:
	int attack_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	AtkUpDrop();
	~AtkUpDrop();
};


#endif // !DROP_H
