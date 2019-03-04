#include "drop.h"

//Public
Drop::Drop(glm::vec3 creation_pos, ObjectID id) : ObjectClass(creation_pos, id) {
	//Class is abstract
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
	//this->health_ = GlobalSettings::Access()->ValueOf("DROP_HP_RESTORE");
	this->health_ = DROP_HP_RESTORE;
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
	//this->health_ = GlobalSettings::Access()->ValueOf("DROP_HP_UP");
	this->health_ = DROP_HP_UP;
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
	//this->attack_ = GlobalSettings::Access()->ValueOf("DROP_ATK_UP");
	this->attack_ = DROP_ATK_UP;
}

AtkUpDrop::~AtkUpDrop() {}

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
	//this->attack_ = GlobalSettings::Access()->ValueOf("DROP_ATK_UP");
	this->key_ = true;
}

KeyDrop::~KeyDrop() {}