#include "player_character.h"
#include "../../Core/sound_unit.h"

//Private--------------------------------------------------
void PlayerCharacter::AlterVelocity() {
	//Get the player's current velocity
	glm::vec3 velocity = this->GetVelocityVec();

	//Then alter the velocity towards the direction the player is facing
	velocity.x += this->move_acceleration_*this->looking_towards_x_;

	//If velocity exceeds top_speed clamp it down
	float positive_velocity = velocity.x * this->looking_towards_x_;
	if (positive_velocity > this->move_top_speed_) {
		velocity.x = this->move_top_speed_ * this->looking_towards_x_;
	}

	//Finally set velocity
	this->SetVelocityVec(velocity);
}


//Public---------------------------------------------------
PlayerCharacter::PlayerCharacter(glm::vec3 start_pos)
	: Character(start_pos,
		OBJECT_ID_PLAYER_IDLE,
		(int)GlobalSettings::Access()->ValueOf("PLAYER_START_HP"),
		(int)GlobalSettings::Access()->ValueOf("PLAYER_START_ATK")
	) {
	this->move_top_speed_ = GlobalSettings::Access()->ValueOf("PLAYER_MOVE_VELOCITY");
	this->move_acceleration_rate_ = GlobalSettings::Access()->ValueOf("PLAYER_MOVE_ACCELERATION_RATE");
	this->move_acceleration_ = this->move_top_speed_ / this->move_acceleration_rate_;
	this->jump_speed_ = GlobalSettings::Access()->ValueOf("PLAYER_JUMP_VELOCITY");

	this->ability_ptr_ = new Ability();
	//this->ability_ptr_ = new DoubleJump();
	//this->ability_ptr_ = new Dash();

	this->weapon_ptr_ = new Sword();
	//this->weapon_ptr_ = new Axe();

	animation_state_ = ANIMATION_STATE_PLAYER_IDLE;

	//Airborne fix
	this->jump_cd_ptr_ = new CooldownClass(GlobalSettings::Access()->ValueOf("PLAYER_JUMP_COOLDOWN"));
	this->airborne_cd_ptr_ = new CooldownClass(GlobalSettings::Access()->ValueOf("PLAYER_AIRBORNE_COOLDOWN"));

	//Set the base scale of this type of unit
	this->SetScale(2.0f);
	this->SetOffsets(2.0f, 2.0f);

	health_last_frame_ = (int)GlobalSettings::Access()->ValueOf("PLAYER_START_HP");
}

PlayerCharacter::~PlayerCharacter() {
	delete this->ability_ptr_;
	delete this->weapon_ptr_;
	delete this->jump_cd_ptr_;
	delete this->airborne_cd_ptr_;
}

AbilityID PlayerCharacter::GetAbilityID() const {
	return this->ability_ptr_->GetID();
}

WeaponID PlayerCharacter::GetWeaponID() const {
	return this->weapon_ptr_->GetID();
}

int PlayerCharacter::GetNumOfKeys() const {
	return this->num_of_keys_;
}

Weapon* PlayerCharacter::GetWeapon() const {
	return this->weapon_ptr_;
}

void PlayerCharacter::MoveLeft() {
	//Set player looking towards negative x
	this->looking_towards_x_ = -1;

	//If the player already exceeds its top movement speed in that direction
	//we do not do anything as we do not want to overwrite the speed
	if (this->GetVelocityVec().x < -this->move_top_speed_) { return; }

	//Change speed in accordance with facing direction
	this->AlterVelocity();
}

void PlayerCharacter::MoveRight() {
	//Set player as looking towards positive x
	this->looking_towards_x_ = 1;

	//If the player already exceeds its top movement speed in that direction
	//we do not do anything as we do not want to overwrite the speed
	if (this->GetVelocityVec().x > this->move_top_speed_) { return; }

	//Change speed in accordance with facing direction
	this->AlterVelocity();
}

void PlayerCharacter::Jump() {

	//If player is not in the air	AND	jump cd is 0
	if (!this->airborne_	&&	this->jump_cd_ptr_->IsOffCooldown()) {
		//Begin jump cd
		this->jump_cd_ptr_->BeginCooldown();

		//Set player velocity towards positive y
		glm::vec3 new_velocity = this->GetVelocityVec();
		new_velocity.y = this->jump_speed_;
		this->SetVelocityVec(new_velocity);
	}
	//We now know the player to be in the air so we check if they have double jump
	else if (this->ability_ptr_->GetID() == ABILITY_DOUBLE_JUMP) {
		//If they do we call for the execution of that ability
		this->ability_ptr_->ExecuteAbility(*this);
	}

	return;
}

void PlayerCharacter::UseAbility() {
	this->ability_ptr_->ExecuteAbility(*this);
}

int PlayerCharacter::UseWeapon(Character& in_target) {
	return this->weapon_ptr_->ExecuteWeapon(*this, in_target);
}

void PlayerCharacter::CalculateAnimationState(float in_deltatime, bool is_attacking) {

	if (is_attacking) {
		animation_state_ = ANIMATION_STATE_IS_ATTACKING;
		animation_timeline_ = 0.0f;
	}

	if (animation_state_ == ANIMATION_STATE_IS_ATTACKING) {
		animation_timeline_ += in_deltatime;

		if (weapon_ptr_->GetID() == WEAPON_SWORD) {
			if (animation_timeline_ < 0.1f) {
				SetObjectID(OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_1);
			}
			else if (animation_timeline_ >= 0.1f && animation_timeline_ <= 0.15f) {
				SetObjectID(OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_2);
			}
			else if (animation_timeline_ > 0.15f && animation_timeline_ <= 0.3f) {
				SetObjectID(OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_3);
			}
			else if (animation_timeline_ > 0.3f) {
				animation_state_ = ANIMATION_STATE_PLAYER_IDLE;
			}
		}
		else if (weapon_ptr_->GetID() == WEAPON_AXE) {
			if (animation_timeline_ < 0.1f) {
				SetObjectID(OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_1);
			}
			else if (animation_timeline_ >= 0.1f && animation_timeline_ <= 0.15f) {
				SetObjectID(OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_2);
			}
			else if (animation_timeline_ > 0.15f && animation_timeline_ <= 0.3f) {
				SetObjectID(OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_3);
			}
			else if (animation_timeline_ > 0.3f) {
				animation_state_ = ANIMATION_STATE_PLAYER_IDLE;
			}
		}
	}
	else if (IsAirborne() && (GetVelocityVec().y < -65.0f || GetVelocityVec().y > 0.0f)) {
		animation_state_ = ANIMATION_STATE_PLAYER_JUMP;
		SetObjectID(OBJECT_ID_PLAYER_JUMP);
	}
	else if (abs(GetVelocityVec().x) > 0.0f) {
		animation_state_ = ANIMATION_STATE_PLAYER_WALK;
		animation_timeline_ += in_deltatime;
		if (animation_timeline_ > 0.13f) {
			if (id_ == OBJECT_ID_PLAYER_LEFT_WALK_1) {
				SetObjectID(OBJECT_ID_PLAYER_LEFT_WALK_2);
			}
			else if (id_ == OBJECT_ID_PLAYER_LEFT_WALK_2) {
				SetObjectID(OBJECT_ID_PLAYER_LEFT_WALK_3);
			}
			else if (id_ == OBJECT_ID_PLAYER_LEFT_WALK_3) {
				SetObjectID(OBJECT_ID_PLAYER_RIGHT_WALK_1);
			}
			else if (id_ == OBJECT_ID_PLAYER_RIGHT_WALK_1) {
				SetObjectID(OBJECT_ID_PLAYER_RIGHT_WALK_2);
			}
			else if (id_ == OBJECT_ID_PLAYER_RIGHT_WALK_2) {
				SetObjectID(OBJECT_ID_PLAYER_RIGHT_WALK_3);
			}
			else {
				SetObjectID(OBJECT_ID_PLAYER_LEFT_WALK_1);
			}
			animation_timeline_ = 0.0f;
		}
	}

	else {
		animation_state_ = ANIMATION_STATE_PLAYER_IDLE;
		SetObjectID(OBJECT_ID_PLAYER_IDLE);
	}
}

Ability* PlayerCharacter::SwapAbility(Ability* in_ability_ptr) {

	//Save the pointer to the old ability
	Ability* old_ability_ptr = this->ability_ptr_;

	//Save the pointer to the new ability
	this->ability_ptr_ = in_ability_ptr;

	//Return the old ability
	return old_ability_ptr;
}

Weapon* PlayerCharacter::SwapWeapon(Weapon* in_weapon_ptr) {
	//Save the pointer to the old weapon
	Weapon* old_weapon_ptr = this->weapon_ptr_;

	//Save the pointer to the new weapon
	this->weapon_ptr_ = in_weapon_ptr;

	//Return the old weapon
	return old_weapon_ptr;
}

void PlayerCharacter::SetAirborne(bool in_air) {

	//Airborne fix
	if (in_air	&&	this->airborne_cd_ptr_->IsOffCooldown()) {
		this->airborne_ = true;
	}
	else {
		if (this->airborne_cd_ptr_->IsOffCooldown()) {
			this->airborne_cd_ptr_->BeginCooldown();
		}
		this->airborne_ = false;
	}

	//If the player just was put out of the air
	//and it has a double jump, set the double jump to available
	if (!in_air && (this->ability_ptr_->GetID() == ABILITY_DOUBLE_JUMP)) {
		((DoubleJump*)this->ability_ptr_)->available_ = true;
	}

}

void PlayerCharacter::UpdateStatus(const float& in_deltatime) {

	//Check if the current ability is a cooldown ability
	CooldownClass* cd_class_ptr = dynamic_cast<CooldownClass*>(this->ability_ptr_);
	if (cd_class_ptr != NULL) {
		//If it is, update its cooldown
		cd_class_ptr->UpdateCooldown(in_deltatime);
	}

	//Update weapon cooldown
	this->weapon_ptr_->UpdateCooldown(in_deltatime);

	//Airborne fix
	this->jump_cd_ptr_->UpdateCooldown(in_deltatime);
	this->airborne_cd_ptr_->UpdateCooldown(in_deltatime);
}

void PlayerCharacter::IncreaseKeys() {
	//Increase keys by one
	this->num_of_keys_++;
}

void PlayerCharacter::LoadPlayerSounds() {
	sound_atk_1_.LoadSound((char*)"../Resources/Audio/swosh1.wav");
	sound_atk_2_.LoadSound((char*)"../Resources/Audio/swosh5.wav");
	sound_atk_3_.LoadSound((char*)"../Resources/Audio/swosh6.wav");
	sound_axe_1_.LoadSound((char*)"../Resources/Audio/swosh_axe1.wav");
	sound_axe_2_.LoadSound((char*)"../Resources/Audio/swosh_axe2.wav");
	damaged_sound_.LoadSound((char*)"../Resources/Audio/player_damage.wav");
	chest_sound_.LoadSound((char*)"../Resources/Audio/Chest.wav");
}

void PlayerCharacter::PlaySound(int sound_index) {
	WeaponID id_index = GetWeaponID();

	if (id_index == WeaponID::WEAPON_AXE)
	{
		switch (sound_index)
		{
		case -1:
			sound_axe_1_.PlaySound();
			break;
		case 0:
			sound_axe_2_.PlaySound();
			break;
		case 1:
			sound_axe_2_.PlaySound();
			break;
		default:
			break;
		}
	}
	else if (id_index == WeaponID::WEAPON_SWORD)
	{
		switch (sound_index)
		{
		case -1:
			sound_atk_1_.PlaySound();
			break;
		case 0:
			sound_atk_2_.PlaySound();
			break;
		case 1:
			sound_atk_3_.PlaySound();
			break;
		default:
			break;
		}
	}


}

void PlayerCharacter::PlayChestSound() {
	chest_sound_.PlaySound();
}

bool PlayerCharacter::DamagedSinceLastFrame() {
	int actual_health = GetCurrentHealth();
	//std::cout << "check health" << std::endl;
	// Player is being damaged
	if (health_last_frame_ > actual_health)
	{
		damaged_sound_.PlaySound();
		health_last_frame_ = actual_health;
		//std::cout << "damaged confirmed" << std::endl;
		return true;
	}

	health_last_frame_ = actual_health;
	return false;
}
