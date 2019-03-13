#ifndef DROP_H
#define DROP_H

#include "../object_class.h"
#include "../Character/player_character.h"
#include "../../global_settings.h"
#include "../Character/ability.h"
#include "../Character/weapon.h"

//---------------------------------------------------------
//-----------------ABSTRACT-BASE-CLASS---------------------
//---------------------------------------------------------

class Drop : public ObjectClass {
private:
	virtual bool TriggerEvent(PlayerCharacter& in_player) = 0;

protected:
	bool swappable_;

public:
	Drop(glm::vec3 creation_pos, ObjectID id);
	~Drop();

	bool CheckCollision(PlayerCharacter& in_player);
	bool IsSwappable();

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

class SpdUpDrop : public Drop {
private:
	int speed_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	SpdUpDrop(glm::vec3 creation_pos);
	~SpdUpDrop();
};

//---------------------------------------------------------

class DashDrop : public Drop {
private:
	bool TriggerEvent(PlayerCharacter& in_player);

public:
	DashDrop(glm::vec3 creation_pos);
	~DashDrop();
};

//---------------------------------------------------------

class DoubleJumpDrop : public Drop {
private:
	bool TriggerEvent(PlayerCharacter& in_player);

public:
	DoubleJumpDrop(glm::vec3 creation_pos);
	~DoubleJumpDrop();
};

//---------------------------------------------------------

class SwordDrop : public Drop {
private:
	bool TriggerEvent(PlayerCharacter& in_player);

public:
	SwordDrop(glm::vec3 creation_pos);
	~SwordDrop();
};

//---------------------------------------------------------

class AxeDrop : public Drop {
private:
	bool TriggerEvent(PlayerCharacter& in_player);

public:
	AxeDrop(glm::vec3 creation_pos);
	~AxeDrop();
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
	float target_coordinate_x_;
	float target_coordinate_y_;
	float target_coordinate_z_;

	bool TriggerEvent(PlayerCharacter& in_player);

public:
	BossDoor(glm::vec3 creation_pos);
	~BossDoor();

	void SpinDrop(const float& in_deltatime);
};


#endif // !DROP_H
