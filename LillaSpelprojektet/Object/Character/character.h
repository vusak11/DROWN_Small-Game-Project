#ifndef CHARACTER_H
#define CHARACTER_H

#include "../object_class.h"

// This class is used as a base of each character in the game. 
// Example:
//    class Player : Character
//    class FireGuy : Character

class Character : public ObjectClass {
private:
	//Character Stats
	int health_;
	int power_ = 1;

	/*
	//Character Basic Metadata
						//NTS: Player can have more advanced stuff such as
	bool attacking_;				//checks for which powerups they have, e.g. double jump
	bool invincible_;
	*/

public:
	Character(glm::vec3 start_pos, ObjectID id);
	~Character();


	void SetHealth(int const health);		//WIP: Sets/Gets redundant?
	void SetPower(int const power);
	int GetHealth();
	int GetPower();
};

#endif // !CHARACTER_H



