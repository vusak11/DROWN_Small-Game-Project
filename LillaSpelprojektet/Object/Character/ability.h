#ifndef ABILITY_H
#define ABILITY_H

//#include "player_character.h"
#include "cooldown_class.h"
#include "../../global_settings.h"


//Forward declaration
class PlayerCharacter;

//---------------------------------------------------------
//----------------------BASE-CLASS-------------------------
//---------------------------------------------------------

class Ability {
protected:
	AbilityID id_;
public:
	Ability();
	~Ability();

	AbilityID GetID() const;

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
