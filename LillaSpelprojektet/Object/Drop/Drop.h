#ifndef DROP_H
#define DROP_H

#include "../object_class.h"
#include "../Character/player_character.h"
#include "../../global_settings.h"

//---------------------------------------------------------
//-----------------ABSTRACT-BASE-CLASS---------------------
//---------------------------------------------------------

class Drop : public ObjectClass {
private:
	virtual bool TriggerEvent(PlayerCharacter& in_player) = 0;

public:
	Drop(glm::vec3 creation_pos, ObjectID id);
	~Drop();

	bool CheckCollision(PlayerCharacter& in_player);

	virtual void SpinDrop(const float& in_deltatime);
};

//---------------------------------------------------------
//--------------------DERIVED-CLASSES----------------------
//---------------------------------------------------------

class HPRestoreDrop : public Drop {
private:
	int health_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	HPRestoreDrop(glm::vec3 creation_pos);
	~HPRestoreDrop();
};

//---------------------------------------------------------

class HPUpDrop : public Drop {
private:
	int health_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	HPUpDrop(glm::vec3 creation_pos);
	~HPUpDrop();
};

//---------------------------------------------------------

class AtkUpDrop : public Drop {
private:
	int attack_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	AtkUpDrop(glm::vec3 creation_pos);
	~AtkUpDrop();
};

//---------------------------------------------------------

class KeyDrop : public Drop {
private:
	bool TriggerEvent(PlayerCharacter& in_player);

public:
	KeyDrop(glm::vec3 creation_pos);
	~KeyDrop();
};

//---------------------------------------------------------

class BossDoor : public Drop {
private:
	int keys_required_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	BossDoor(glm::vec3 creation_pos);
	~BossDoor();

	void SpinDrop(const float& in_deltatime);
};


#endif // !DROP_H
