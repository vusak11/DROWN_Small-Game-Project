#include "weapon.h"

Weapon::Weapon(
	WeaponID id,
	int attack_power,
	float cooldown,
	float hitbox_offset_x,
	float hitbox_offset_y) 
	: CooldownClass(cooldown) {
	this->id_ = id;
	this->attack_power_ = attack_power;

	this->weapon_box_.SetOffsets(hitbox_offset_x, hitbox_offset_y);
}

Weapon::~Weapon() {

}

WeaponID Weapon::GetID() const {
	return this->id_;
}

int Weapon::GetAttackPower() const {
	return this->attack_power_;
}

int Weapon::ExecuteWeapon(Character& in_attacker, Character& in_target) {
	
	//Returns	-1	if attack unsuccessful
	//			 0	if it was a hit
	//			 1	if the hit killed the target

	// TBA in later Task ;)
	//Check if the weapon is off cooldown
	//	if (!this->IsOffCooldown()) {
	//		//If it isn't return -1
	//		return -1;
	//	}

	//Otherwise set the cooldown as we now make an attack
	this->BeginCooldown();

	//Get the position of the attacker
	glm::vec3 box_center = in_attacker.GetPosition();

	//Offset the center of the box with half its
	//width in the direction the attacker is facing
	box_center.x += in_attacker.GetFacingDirection() * this->weapon_box_.GetXOffset();

	//Update the weapon box with the new center
	this->weapon_box_.SetPosition(box_center);

	//Check is the weapon's hitbox overlap with the target's hitbox
	if (!in_target.CheckCollision(this->weapon_box_.GetPoints())) {
		//If it doesn't return -1

		return -1;
	}

	//If it does apply the sum of the weapon's damage and the attacker's
	//attack power to the target
	int damage = this->attack_power_ + in_attacker.GetAttackPower();
	
	return in_target.TakeDamage(damage);
}
//---------------------------------------------------------
Sword::Sword()
	: Weapon(
		WEAPON_SWORD,
		GlobalSettings::Access()->ValueOf("WEAPON_SWORD_DAMAGE"),
		GlobalSettings::Access()->ValueOf("WEAPON_SWORD_COOLDOWN"),
		GlobalSettings::Access()->ValueOf("WEAPON_SWORD_HITBOX_OFFSET_X"),
		GlobalSettings::Access()->ValueOf("WEAPON_SWORD_HITBOX_OFFSET_Y")
	) {

}

Sword::~Sword() {

}

int Sword::ExecuteWeapon(Character& in_attacker, Character& in_target) {
	//This weapon only uses the default damaging function
	return this->Weapon::ExecuteWeapon(in_attacker, in_target);
}
//---------------------------------------------------------
Axe::Axe()
	: Weapon(
		WEAPON_AXE,
		GlobalSettings::Access()->ValueOf("WEAPON_AXE_DAMAGE"),
		GlobalSettings::Access()->ValueOf("WEAPON_AXE_COOLDOWN"),
		GlobalSettings::Access()->ValueOf("WEAPON_AXE_HITBOX_OFFSET_X"),
		GlobalSettings::Access()->ValueOf("WEAPON_AXE_HITBOX_OFFSET_Y")
	) {

	this->knock_back_ = GlobalSettings::Access()->ValueOf("WEAPON_AXE_KNOCKBACK");

}

Axe::~Axe() {

}

int Axe::ExecuteWeapon(Character& in_attacker, Character& in_target) {
	
	//Use the weapon
	int outcome = this->Weapon::ExecuteWeapon(in_attacker, in_target);

	//If attack connects, apply knock back
	glm::vec3 direction;
	if (outcome != -1) {

		//Align directional vector from attacker to target
		direction = in_target.GetPosition() - in_attacker.GetPosition();

		//Normailize vector
		direction = glm::normalize(direction);

		//TEST
		//If y-direction is less than 45deg
		//(y = 1/sqrt(2) = 0.707..)
		//set y to be that
		if (direction.y < 0.707) { direction.y = 0.707; }
		//TEST

		//Scale direction with knockback
		direction *= this->knock_back_;

		//Set the target's velocity
		in_target.SetVelocityVec(direction);

	}

	return outcome;
}
//---------------------------------------------------------