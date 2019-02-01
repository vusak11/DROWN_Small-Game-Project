#pragma once

#include "object_class.h"

// This class is used as a base of each character in the game. 
// Example:
//    class Player : Character
//    class FireGuy : Character

class Character : ObjectClass {
public:
	Character();
	~Character();

	void SetHealth(int const health);
	void SetPower(int const power);

	int GetHealth();
	int GetPower();
private:
	int health_;
	int power_ = 1;
	
};
