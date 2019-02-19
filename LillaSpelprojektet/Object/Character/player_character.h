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
private:
	//friend bool Ability::ExecuteAbility(PlayerCharacter&);
	friend bool DoubleJump::ExecuteAbility(PlayerCharacter& in_player);
	friend bool Dash::ExecuteAbility(PlayerCharacter& in_player);

	//Player stats
	float top_speed_;
	float jump_speed_;

	Weapon weapon_;
	Ability* ability_ptr_;

public:
	PlayerCharacter(glm::vec3 start_pos);
	~PlayerCharacter();

	void MoveLeft();
	void MoveRight();
	void Jump();

	void SetAirborne(bool in_bool);
	void UpdateStatus(float in_deltatime);		//Updates time related stuff
												//e.g. cooldown on abilities
												//or attack windows

};


#endif // !PLAYER_CHARACTER_H