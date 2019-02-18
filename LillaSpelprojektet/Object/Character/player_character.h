#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "character.h"
#include "ability.h"

//Forward declaraction
//class Ability;

//WIP: Placeholder for proper weapons and abilities
enum WeaponID {
	SWORD
};



struct Weapon {
	WeaponID id;
	int damage;
	float cooldown;
	//Hitbox data
};
//

class PlayerCharacter : public Character {
	//friend class Ability;
	//friend bool Ability::ExecuteAbility(PlayerCharacter&);
private:
	//friend class Ability;
	friend bool Ability::ExecuteAbility(PlayerCharacter&);

	//Player stats
	float top_speed_;
	float jump_speed_;

	Weapon weapon_;
	Ability* ability_;

public:
	//friend class Ability;
	//friend bool Ability::ExecuteAbility(PlayerCharacter&);

	PlayerCharacter(glm::vec3 start_pos);
	~PlayerCharacter();

	void MoveLeft();
	void MoveRight();
	void Jump();

	void SetAirborne(bool in_bool);

};


#endif // !PLAYER_CHARACTER_H
