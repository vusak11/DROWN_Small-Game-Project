#include "drop.h"

//Public
Drop::Drop(glm::vec3 creation_pos, ObjectID id) : ObjectClass(creation_pos, id) {
	//Class is abstract

	this->swappable_ = false; //This should be overwritten in swappable child classes.
}

Drop::~Drop() {}

bool Drop::CheckCollision(PlayerCharacter& in_player) {
	
	//Get the player's hitbox
	BoxPoints player_box = in_player.GetPoints();

	//If this object doesn't collide, return false
	if (!this->Hitbox::CheckCollision(player_box)) { return false; }
	
	//Otherwise trigger its event and return true/false
	return this->TriggerEvent(in_player);
}

bool Drop::IsSwappable() {
	return this->swappable_;
}

void Drop::SpinDrop(const float& in_deltatime) {
	//Turn the model rightwards (positive direction) with adjustment for deltatime
	float turn_radians = this->turn_rate_radians_*in_deltatime*0.1f;
	float new_rotation = this->rotation_around_y_ + turn_radians;

	this->SetRotation(this->rotation_around_x_, new_rotation, this->rotation_around_z_);
	//this->SetRotation(new_rotation, new_rotation, new_rotation);
}

//---------------------------------------------------------
//Private
bool HPRestoreDrop::TriggerEvent(PlayerCharacter& in_player) {

	//Restore health to the player and return false if unsucessful
	if (-1 == in_player.HealDamage(this->health_)) {
		return false;
	}

	//Otherwise return true
	return true;
}

//Public
HPRestoreDrop::HPRestoreDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_HP_RESTORE) {
	this->health_ = GlobalSettings::Access()->ValueOf("DROP_HP_RESTORE");
}

HPRestoreDrop::~HPRestoreDrop() {}

//---------------------------------------------------------
//Private
bool HPUpDrop::TriggerEvent(PlayerCharacter& in_player) {
	//Increase the player's max health, return true is successful
	return in_player.IncreaseMaxHealth(this->health_);
}

//Public
HPUpDrop::HPUpDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_HP_UP) {
	this->health_ = GlobalSettings::Access()->ValueOf("DROP_HP_UP");
}

HPUpDrop::~HPUpDrop() {}

//---------------------------------------------------------
//Private
bool AtkUpDrop::TriggerEvent(PlayerCharacter& in_player) {
	//Increase the player's attack power, return true is successful
	return in_player.IncreaseAttack(this->attack_);
}

//Public
AtkUpDrop::AtkUpDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_ATK_UP) {
	this->attack_ = GlobalSettings::Access()->ValueOf("DROP_ATK_UP");
}

AtkUpDrop::~AtkUpDrop() {}

//---------------------------------------------------------
//Private
bool SpdUpDrop::TriggerEvent(PlayerCharacter& in_player) {
	//Increase the player's attack power, return true is successful
	return in_player.IncreaseSpeed(this->speed_);
}

//Public
SpdUpDrop::SpdUpDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_SPD_UP) {
	this->speed_ = GlobalSettings::Access()->ValueOf("DROP_SPD_UP");
}

SpdUpDrop::~SpdUpDrop() {}

//---------------------------------------------------------
//Private
bool KeyDrop::TriggerEvent(PlayerCharacter& in_player) {
	//Giving the playr a key, then return true
	in_player.IncreaseKeys();
	return true;
}

//Public
KeyDrop::KeyDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_KEY) {
}

KeyDrop::~KeyDrop() {}

//---------------------------------------------------------
//Private
bool BossDoor::TriggerEvent(PlayerCharacter& in_player) {
	//Check if player has enough keys
	if (in_player.GetNumOfKeys() >= this->keys_required_) {

		// SET BOSS STATE

		//If so, move player to boss-room
		in_player.SetPosition(
			GlobalSettings::Access()->ValueOf("DROP_BOSS_DOOR_DESTINATION_X"),
			GlobalSettings::Access()->ValueOf("DROP_BOSS_DOOR_DESTINATION_Y"),
			GlobalSettings::Access()->ValueOf("DROP_BOSS_DOOR_DESTINATION_Z")
		);

		//Then return true
		return true;
	}

	//If not, return false
	return false;
}

//Public
BossDoor::BossDoor(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_DOOR) {

	this->keys_required_ = GlobalSettings::Access()->ValueOf("DROP_NUM_OF_KEYS");

}

BossDoor::~BossDoor() {}

void BossDoor::SpinDrop(const float& in_deltatime) {
	//Empty as the boss-door shouldn't rotate
}

//----------------------------------------------------------
//Private
bool DashDrop::TriggerEvent(PlayerCharacter& in_player) {
	
	//Give the player a dash and catch the ptr to the old ability
	Ability* old_ability_ptr = in_player.SwapAbility(new Dash());

	//Delete the old ability
	delete old_ability_ptr;

	//Return true
	return true;
}

//Public
DashDrop::DashDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_DASH) {
	//This drop is swappable!
	this->swappable_ = true;
}

DashDrop::~DashDrop() {}

//---------------------------------------------------------
//Private
bool DoubleJumpDrop::TriggerEvent(PlayerCharacter& in_player) {
	
	//Give the player a double jump and catch the ptr to the old ability
	Ability* old_ability_ptr = in_player.SwapAbility(new DoubleJump());

	//Delete the old ability
	delete old_ability_ptr;

	//Return true
	return true;
}

//Public
DoubleJumpDrop::DoubleJumpDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_DOUBLE_JUMP) {
	//This drop is swappable!
	this->swappable_ = true;
}

DoubleJumpDrop::~DoubleJumpDrop() {}

