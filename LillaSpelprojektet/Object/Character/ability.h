#ifndef ABILITY_H
#define ABILITY_H

//#include "player_character.h"
#include "cooldown_class.h"

//Forward declaration
class PlayerCharacter;

//---------------------------------------------------------
//----------------------BASE-CLASS-------------------------
//---------------------------------------------------------

enum AbilityID {
	ABILITY_NONE,
	ABILITY_DOUBLE_JUMP,
	ABILITY_DASH
};

class Ability {
public:
	AbilityID id_;
	Ability();
	~Ability();
	virtual bool ExecuteAbility(PlayerCharacter& in_player);
};

//---------------------------------------------------------
//--------------------DERIVED-CLASSES----------------------
//---------------------------------------------------------

class DoubleJump : public Ability {
public:
	bool available_;
	DoubleJump();
	~DoubleJump();
	bool ExecuteAbility(PlayerCharacter& in_player);
};

//---------------------------------------------------------

class Dash : public Ability, public CooldownClass {
private:
	float dash_velocity_;
public:
	Dash();
	~Dash();
	bool ExecuteAbility(PlayerCharacter& in_player);
};

//---------------------------------------------------------

#endif // !ABILITY_H