#include "object_handler.h"

//Private--------------------------------------------------

std::vector<ObjectClass> CullAndRetrieveNPCs() {

}

std::vector<ObjectClass> CullAndRetrieveDrops() {

}

float ObjectHandler::DistanceBetween(ObjectClass in_object_a, ObjectClass in_object_b) {
	in_object_a.GetPosition();
}

void ObjectHandler::ClearPlayerInput() {
	this->player_input_.left = false;
	this->player_input_.right = false;
	this->player_input_.jump = false;
	this->player_input_.attack = false;
	this->player_input_.use_ability = false;
	this->player_input_.pick_up = false;
}



//Public---------------------------------------------------

ObjectHandler::ObjectHandler() {

}

ObjectHandler::~ObjectHandler() {

}

void ObjectHandler::PlayerMoveLeft() {
	this->player_input_.left = true;
}

void ObjectHandler::PlayerMoveRight() {
	this->player_input_.right = true;
}

void ObjectHandler::PlayerJump() {
	this->player_input_.jump = true;
}

void ObjectHandler::PlayerAttack() {
	this->player_input_.attack = true;
}

void ObjectHandler::PlayerUseAbility() {
	this->player_input_.use_ability;
}

void ObjectHandler::PlayerPickUp() {
	this->player_input_.pick_up;
}

void ObjectHandler::Update() {




	//Reset the inputs
	this->ClearPlayerInput();

	return;
}

