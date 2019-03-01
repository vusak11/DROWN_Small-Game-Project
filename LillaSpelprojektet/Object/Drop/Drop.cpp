#include "drop.h"

Drop::Drop() {
	//Class is abstract
}

Drop::~Drop() {}

bool Drop::CheckCollision(PlayerCharacter& in_player) {
	
	//Get the player's hitbox
	BoxPoints player_box = in_player.GetPoints();

	//If this object doesn't collide, return false
	if (!this->ObjectClass::CheckCollision(player_box)) { false; }
	
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
HPRestoreDrop::HPRestoreDrop() {
	//this->health_ = GlobalSettings::Access()->ValueOf("DROP_HP_RESTORE");
	this->health_ = DROP_HP_RESTORE;
}

HPRestoreDrop::~HPRestoreDrop() {}

//---------------------------------------------------------
//Private::
HPUpDrop::HPUpDrop() {
	//this->health_ = GlobalSettings::Access()->ValueOf("DROP_HP_UP");
	this->health_ = DROP_HP_UP;
}

HPUpDrop::~HPUpDrop() {}

bool HPUpDrop::TriggerEvent(PlayerCharacter& in_player) {
	//Increase the player's max health, return true is successful
	return in_player.IncreaseMaxHealth(this->health_);
}

//---------------------------------------------------------

AtkUpDrop::AtkUpDrop() {
	//this->attack_ = GlobalSettings::Access()->ValueOf("DROP_ATK_UP");
	this->attack_ = DROP_ATK_UP;
}

AtkUpDrop::~AtkUpDrop() {}

bool AtkUpDrop::TriggerEvent(PlayerCharacter& in_player) {
	//Increase the player's attack power, return true is successful
	return in_player.IncreaseAttack(this->attack_);
}