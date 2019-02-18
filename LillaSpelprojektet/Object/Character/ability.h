#ifndef ABILITY_H
#define ABILITY_H

//#include "player_character.h"

//Forward declaration
class PlayerCharacter;

enum AbilityID {
	NONE,
	DOUBLE_JUMP,
	DASH
};

class Ability {
public:
	AbilityID id_;
	Ability();
	~Ability();
	virtual bool ExecuteAbility(PlayerCharacter& in_player) = 0;
};


//---------------------------------------------------------
class DoubleJump : public Ability {
public:
	bool available_;
	DoubleJump();
	~DoubleJump();
	bool ExecuteAbility(PlayerCharacter& in_player);
};
//---------------------------------------------------------
class Dash : public Ability {
public:
	float cooldown_;
	Dash();
	~Dash();
	bool ExecuteAbility(PlayerCharacter& in_player);
};
//---------------------------------------------------------

#endif // !ABILITY_H
