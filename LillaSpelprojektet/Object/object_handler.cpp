#include "object_handler.h"

//Private--------------------------------------------------

bool ObjectHandler::RemoveObject(ObjectClass in_object, std::vector<ObjectClass>& in_object_vector) {
	
	//Go through the array
	for (unsigned int i = 0; i < in_object_vector.size(); i++) {
		//See if the given object exists within it
		if (in_object == in_object_vector.at(i)) {
			//if it does, remove that element and return true
			in_object_vector.erase(in_object_vector.begin() + i);
			return true;
		}
	}

	//If the object wasn't found return false
	return false;
}

std::vector<ObjectClass*> ObjectHandler::CullAndRetrieveObjects(std::vector<ObjectClass>& in_object_vector) {
	
	//NTS:	Function takes a REFERENCE to a std::vector with ObjectClass:es
	//		It creates a std::vector holding POINTERS to ObjectClass:es
	//		It fills the std::vector with POINTERS to the OBJECTS that the REFERENCES refer to
	//		It then returns a std::vector of POINTERS
	
	//Set up a vector to hold pointers to all relevant objects
	std::vector<ObjectClass*> nearby_objects_ptr_vector;

	//Go through all NPCs
	for (unsigned int i = 0; i < in_object_vector.size(); i++) {
		//See which Objects are within OBJECT_CULLING_DISTANCE of the player
		if (this->DistanceBetween(this->player_object_, in_object_vector.at(i)) < OBJECT_CULLING_DISTANCE) {
			//Those that are close enough are added to the vector
			nearby_objects_ptr_vector.push_back(&in_object_vector.at(i));
		}
	}

	//Return the vector
	return nearby_objects_ptr_vector;
}

/*
std::vector<ObjectClass> ObjectHandler::CullAndRetrieveDrops() {

}


float ObjectHandler::DistanceBetween(ObjectClass in_object_a, ObjectClass in_object_b) {
	//Returns distance between two objects
	return glm::distance(in_object_a.GetPosition(), in_object_b.GetPosition());
}
*/

float ObjectHandler::DistanceBetween(const ObjectClass& in_object_a, const ObjectClass& in_object_b) {
	//Returns distance between two objects

	//NTS:
	//Alternate function to DistanceBetween(ObjectClass in_object_a, ObjectClass in_object_b)
	//using constant references to keep things safe while avoiding copying

	return glm::distance(in_object_a.GetPosition(), in_object_b.GetPosition());
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

	
	//DeterminePlayerAction();
	
	//DetermineNPCAction(/*vector.at(i)*/);

	//ResolvePlayerAction();
	
	//ResolveNPCAction(/*vector.at(i)*/);

	//ResolveDropBehaviour(in_drop);

	//Reset the inputs
	this->ClearPlayerInput();

	return;
}

