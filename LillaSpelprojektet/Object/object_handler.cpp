#include "object_handler.h"

//Private--------------------------------------------------
bool ObjectHandler::ClearPtrVector(std::vector<ObjectClass*>& in_vec) {
	//While vector is not empty
	while (!in_vec.empty()) {
		//Delete the first object in vector
		delete in_vec.at(0);
		//then erase the first entry
		in_vec.erase(this->npc_ptr_vector_.begin() + 0);
	}

	return in_vec.empty();
}

bool ObjectHandler::RemoveObject(const ObjectClass* in_object_ptr, std::vector<ObjectClass*>& in_object_ptr_vector) {
	
	//WARNING: This function deletes the given object on a success
	//If a pointer has been saved externally it will lead to a trash location
	//Taking a reference to the pointer and setting it to NULL will not save
	//us since the pointer might have been copied outside

	//Go through the array
	for (unsigned int i = 0; i < in_object_ptr_vector.size(); i++) {
		//See if the given object exists within it
		if (in_object_ptr == in_object_ptr_vector.at(i)) {
			//if it does, delete pointer to object, then remove that element from the vector and finally return true
			delete in_object_ptr_vector.at(i);
			in_object_ptr_vector.erase(in_object_ptr_vector.begin() + i);
			return true;
		}
	}

	//If the object wasn't found return false
	return false;
}

std::vector<ObjectClass*> ObjectHandler::CullAndRetrieveObjects(const std::vector<ObjectClass*>& in_object_vector) const {
	
	//NTS:	Function takes a REFERENCE to a std::vector with OBJECTCLASS POINTERS
	//		It creates a std::vector holding OBJECTCLASS POINTERS
	//		It fills the std::vector with OBJECTCLASS POINTERS that the REFERENCES refer to
	//		It then returns a std::vector of POINTERS
	
	//Set up a vector to hold pointers to all relevant objects
	std::vector<ObjectClass*> nearby_objects_ptr_vector;

	//Go through all NPCs
	for (unsigned int i = 0; i < in_object_vector.size(); i++) {
		//See which Objects are within OBJECT_CULLING_DISTANCE of the player
		if (this->DistanceBetween(this->player_ptr_, in_object_vector.at(i)) < OBJECT_CULLING_DISTANCE) {
			//Those that are close enough are added to the vector
			nearby_objects_ptr_vector.push_back(in_object_vector.at(i));
		}
	}

	//Return the vector
	return nearby_objects_ptr_vector;
}

float ObjectHandler::DistanceBetween(const ObjectClass* in_object_a, const ObjectClass* in_object_b) const {
	//Returns distance between two objects

	//NTS:
	//Alternate function to DistanceBetween(ObjectClass in_object_a, ObjectClass in_object_b)
	//using constant references to keep things safe while avoiding copying

	return glm::distance(in_object_a->GetPosition(), in_object_b->GetPosition());
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
	this->player_ptr_ = NULL;
}

ObjectHandler::~ObjectHandler() {
	delete this->player_ptr_;
	this->ClearPtrVector(this->npc_ptr_vector_);
	this->ClearPtrVector(this->drop_ptr_vector_);

}

void ObjectHandler::InitializeObjectHandler() {

	this->player_ptr_ = new ObjectClass(glm::vec3(0.0f, 0.0f, 0.0f), OBJECT_ID_PLAYER);

	this->TestObjectHandler();		//NTS: Just for testing


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

