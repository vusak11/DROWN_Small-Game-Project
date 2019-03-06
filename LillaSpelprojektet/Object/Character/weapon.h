#ifndef WEAPON_H
#define WEAPON_H

#include "cooldown_class.h"
#include "character.h"
#include "../hitbox.h"
#include "../../global_settings.h"


//---------------------------------------------------------
//----------------------BASE-CLASS-------------------------
//---------------------------------------------------------

class Weapon : public CooldownClass {
private:
	int attack_power_;
	Hitbox weapon_box_;

protected:
	WeaponID id_;

public:
	Weapon(
		WeaponID id,
		int attack_power,
		float cooldown,
		float hitbox_offset_x,	//These offsets are essentially given
		float hitbox_offset_y	//in half of the side of the actual box
	);
	~Weapon();

	WeaponID GetID() const;
	int GetAttackPower() const;


	virtual int UseWeapon(Character& in_attacker, Character& in_target);	//Returns	-1	if attack unsuccessful
																			//			 0	if it was a hit
																			//			 1	if the hit killed the target
};

//---------------------------------------------------------
//--------------------DERIVED-CLASSES----------------------
//---------------------------------------------------------

class Sword : public Weapon {
public:
	Sword();
	~Sword();

	int UseWeapon(Character& in_attacker, Character& in_target);
};

//---------------------------------------------------------

class Axe : public Weapon {
private:
	float knock_back_;
public:
	Axe();
	~Axe();

	int UseWeapon(Character& in_attacker, Character& in_target);
};

//---------------------------------------------------------

#endif // !WEAPON_H
