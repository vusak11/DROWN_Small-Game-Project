#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "character.h"

//WIP: Placeholder for proper weapons and abilities
enum WeaponID {
	SWORD
};

enum AbilityID {
	NONE,
	DOUBLE_JUMP
};

struct Weapon {
	WeaponID id;
	int damage;
	float cooldown;
	//Hitbox data
};

struct Ability {
	AbilityID id;
	float cooldown;
};

//

class PlayerCharacter : public Character {
private:
	//Player stats
	float top_speed_;
	float jump_speed_;

	Weapon weapon_;
	Ability ability_;
	

public:
	PlayerCharacter(glm::vec3 start_pos);
	~PlayerCharacter();

	void MoveLeft();
	void MoveRight();
	void Jump();

	void SetAirborne(bool in_bool);

};


#endif // !PLAYER_CHARACTER_H
