#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "character.h"
#include "ability.h"

//Forward declaraction
//class Ability;

//WIP: Placeholder for proper weapons and abilities

struct Weapon {
	WeaponID id;
	int damage;
	float cooldown;
	//Hitbox data
};
//

class PlayerCharacter : public Character {
private:
	//Player stats
	float move_top_speed_;
	float move_acceleration_;
	float jump_speed_;

	Ability* ability_ptr_;
	Weapon weapon_;

	//friend bool Ability::ExecuteAbility(PlayerCharacter&);
	friend bool DoubleJump::ExecuteAbility(PlayerCharacter& in_player);
	friend bool Dash::ExecuteAbility(PlayerCharacter& in_player);

	void AlterVelocity();

public:
	PlayerCharacter(glm::vec3 start_pos);
	~PlayerCharacter();

	AbilityID GetAbilityID() const;
	WeaponID GetWeaponID() const;

	void MoveLeft();
	void MoveRight();
	void Jump();
	void UseAbility();

	void SetAirborne(bool in_bool);
	void UpdateStatus(const float& in_deltatime);		//Updates time related stuff
														//e.g. cooldown on abilities
														//or attack windows

};


#endif // !PLAYER_CHARACTER_H
