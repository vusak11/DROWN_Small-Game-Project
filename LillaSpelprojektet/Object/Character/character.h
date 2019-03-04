#ifndef CHARACTER_H
#define CHARACTER_H

#include "../object_class.h"

#include <stdexcept>

// This class is used as a base of each character in the game. 
// Example:
//    class Player : Character
//    class FireGuy : Character

class Character : public ObjectClass {
protected:
	//Character Stats
	int max_health_;
	int current_health_;
	int attack_power_;
	bool has_key_ = false;
	//
	int looking_towards_x_;

	/*
	//Character Basic Metadata
						//NTS: Player can have more advanced stuff such as
	bool attacking_;	//checks for which powerups they have, e.g. double jump
	bool invincible_;
	*/

public:
	Character(glm::vec3 start_pos, ObjectID id);
	~Character();

	//Character Stat Stuff------------------------------------
	void SetMaxHealth(int in_hp);		//Throws invalid argument
	void SetCurrentHealth(int in_hp);	//Throws invalid argument
	void SetAttackPower(int in_atk);	//Throws invalid argument
	
	int GetMaxHealth() const;
	int GetCurrentHealth() const;
	int GetAttackPower() const;

	int TakeDamage(int in_dmg);		//Returns	-1 if in-value is negative,
	int HealDamage(int in_heal);	//			 0 if operation was successful
									//			 1 if cap (max hp/0 hp) was hit
	
	bool IncreaseMaxHealth(int in_hp);	//Returns	true if operation was allowed
	bool IncreaseAttack(int in_atk);	//			false if operation failed
	bool SetKeyStatus(bool in_key);
	//Character Movement Stuff-----------------------------
	void TurnLeft(const float& in_deltatime);
	void TurnRight(const float& in_deltatime);
};

#endif // !CHARACTER_H



