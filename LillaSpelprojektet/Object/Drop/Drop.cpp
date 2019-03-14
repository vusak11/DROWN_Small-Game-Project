#include "drop.h"

//Public
Drop::Drop(glm::vec3 creation_pos, ObjectID id) : ObjectClass(creation_pos, id) {
	//Class is abstract

	this->swappable_ = false; //This should be overwritten in swappable child classes.

	this->random_spawns_ = 0;
	this->set_spawns_ptr_ = NULL;

	//Set the base scale of this type of unit
	this->SetScale(2.0f);
}

Drop::~Drop() {
	delete this->set_spawns_ptr_;
}

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

int Drop::ConsumeNumOfRandomSpawns() {
	//This function return the number of randoms spawns the
	//ObjectHandler should create. It also resets its own counter
	//while doing so
	int temp = this->random_spawns_;
	this->random_spawns_ = 0;
	return temp;
}

Drop* Drop::RetrieveSetSpawnPtr() {
	//This function return a pointer to the set
	//drop the function held by this drop, then
	//set that pointer to null
	//Note that the function will return null if
	//there is none.
	//Also note that the object the pointer points
	//at will no longer be deleted by this destructor
	//after this function has been called
	Drop* temp = this->set_spawns_ptr_;
	this->set_spawns_ptr_ = NULL;
	return temp;
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

//---------------------------------------------------------
//Private
bool SwordDrop::TriggerEvent(PlayerCharacter& in_player) {

	//Give the player a double jump and catch the ptr to the old ability
	Weapon* old_weapon_ptr = in_player.SwapWeapon(new Sword());

	//Delete the old ability
	delete old_weapon_ptr;

	//Return true
	return true;
}

//Public
SwordDrop::SwordDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_SWORD) {
	//This drop is swappable!
	this->swappable_ = true;
}

SwordDrop::~SwordDrop() {}

//---------------------------------------------------------
//Private
bool AxeDrop::TriggerEvent(PlayerCharacter& in_player) {

	//Give the player a double jump and catch the ptr to the old ability
	Weapon* old_weapon_ptr = in_player.SwapWeapon(new Axe());

	//Delete the old ability
	delete old_weapon_ptr;

	//Return true
	return true;
}

//Public
AxeDrop::AxeDrop(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_AXE) {
	//This drop is swappable!
	this->swappable_ = true;
}

AxeDrop::~AxeDrop() {}

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
			this->target_coordinate_x_,
			this->target_coordinate_y_,
			this->target_coordinate_z_
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

	this->target_coordinate_x_ = GlobalSettings::Access()->ValueOf("DROP_BOSS_DOOR_DESTINATION_X");
	this->target_coordinate_y_ = GlobalSettings::Access()->ValueOf("DROP_BOSS_DOOR_DESTINATION_Y");
	this->target_coordinate_z_ = GlobalSettings::Access()->ValueOf("DROP_BOSS_DOOR_DESTINATION_Z");

}

BossDoor::~BossDoor() {}

void BossDoor::SpinDrop(const float& in_deltatime) {
	//Empty as the boss-door shouldn't rotate
}

//---------------------------------------------------------
//Private
bool Chest::TriggerEvent(PlayerCharacter& in_player) {
	
	//If the chest is already open, don't do anything
	if (this->GetObjectID() == OBJECT_ID_DROP_CHEST_OPEN) {
		return false;
	}

	//Otherwise set it to now be open
	this->SetObjectID(OBJECT_ID_DROP_CHEST_OPEN);

	//Set number of random spawns
	this->random_spawns_ = this->num_of_loot_;

	//Return true
	return true;
}

//Public
Chest::Chest(glm::vec3 creation_pos)
	: Drop(creation_pos, OBJECT_ID_DROP_CHEST_CLOSED) {

	this->num_of_loot_ = GlobalSettings::Access()->ValueOf("DROP_CHEST_NUM_OF_LOOT");

	//---
	//Mess a bit with the look of a chest
	//---
	//Set the base scale of this type of unit
	this->SetScale(1.0f, 2.5f, 1.0f);

	// Move the chest a bit outwards
	glm::vec3 pos = this->GetPosition();
	pos.z += 2.0f;
	this->SetPosition(pos.x, pos.y, pos.z);
}

Chest::~Chest() {}

void Chest::SpinDrop(const float& in_deltatime) {
	//Empty as chests shouldn't rotate
}