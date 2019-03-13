#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "character.h"
#include "ability.h"
#include "weapon.h"

//Forward declaraction
//class Ability;


class PlayerCharacter : public Character {
private:
	//Player stats
	float jump_speed_;

	Ability* ability_ptr_;
	Weapon* weapon_ptr_;
	AnimationState animation_state_;
	float animation_timeline_ = 0;

	int num_of_keys_;

	//friend bool Ability::ExecuteAbility(PlayerCharacter&);
	friend bool DoubleJump::ExecuteAbility(PlayerCharacter& in_player);
	friend bool Dash::ExecuteAbility(PlayerCharacter& in_player);

	void AlterVelocity();

public:
	PlayerCharacter(glm::vec3 start_pos);
	~PlayerCharacter();

	AbilityID GetAbilityID() const;
	WeaponID GetWeaponID() const;
	int GetNumOfKeys() const;

	void MoveLeft();
	void MoveRight();
	void Jump();
	void UseAbility();
	int UseWeapon(Character& in_target);

	void CalculateAnimationState(float delta_time);

	Ability* SwapAbility(Ability* in_ability_ptr);
	
	void SetAirborne(bool in_bool);
	void UpdateStatus(const float& in_deltatime);		//Updates time related stuff
														//e.g. cooldown on abilities
														//or attack windows
	void IncreaseKeys();

};


#endif // !PLAYER_CHARACTER_H
