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


	void SetMaxHealth(int in_hp);
	void SetCurrentHealth(int in_hp);
	void SetAttackPower(int in_atk);
	
	int GetMaxHealth() const;
	int GetCurrentHealth() const;
	int GetAttackPower() const;

	int TakeDamage(int in_dmg);		//Throws	   if in-value is negative,
	int HealDamage(int in_heal);	//Returns	 0 if operation was successful
									//			 1 if cap (max hp/0 hp) was hit
};

#endif // !CHARACTER_H



