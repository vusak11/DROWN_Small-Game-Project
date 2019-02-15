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

std::vector<ObjectClass*> ObjectHandler::CullAndRetrieveObjectPtrs(const std::vector<ObjectClass*>& in_object_vector) const {
	
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

void ObjectHandler::DeterminePlayerAction() {


	//Determine player movement on the x-axis
	if (this->player_input_.left) {
		//Set player velocity towards negative x
		glm::vec3 velocity_change;

		if (this->player_ptr_->object_metadata_.airborne) {
			velocity_change = glm::vec3((float)-PLAYER_X_AIR_SPEED, 0.0f, 0.0f);
		}
		else {
			velocity_change = glm::vec3((float)-PLAYER_X_SPEED, 0.0f, 0.0f);
		}

		this->player_ptr_->AlterVelocityVec(velocity_change);
	}
	if (this->player_input_.right) {
		//Set player velocity towards positive x
		glm::vec3 velocity_change;

		if (this->player_ptr_->object_metadata_.airborne) {
			velocity_change = glm::vec3((float)PLAYER_X_AIR_SPEED, 0.0f, 0.0f);
		}
		else {
			velocity_change = glm::vec3((float)PLAYER_X_SPEED, 0.0f, 0.0f);
		}

		this->player_ptr_->AlterVelocityVec(velocity_change);
	}

	//If input is jump and player is not in the air
	if (this->player_input_.jump && !this->player_ptr_->object_metadata_.airborne) {
		//Set player velocity towards positive y
		glm::vec3 velocity_change = glm::vec3(0.0f, (float)PLAYER_Y_SPEED, 0.0f);
		this->player_ptr_->AlterVelocityVec(velocity_change);
		
		//Then set the object to be airborne
		this->player_ptr_->object_metadata_.airborne = true;
	}

	if (this->player_input_.attack) {
		
	}
	if (this->player_input_.pick_up) {
		glm::vec3 p = player_ptr_->GetPosition();
		//this->player_ptr_->SetPosition(200, 0, p.z);
	}
	if (this->player_input_.use_ability) {

	}
}

void ObjectHandler::ClearPlayerInput() {
	this->player_input_.left = false;
	this->player_input_.right = false;
	this->player_input_.jump = false;
	this->player_input_.attack = false;
	this->player_input_.use_ability = false;
	this->player_input_.pick_up = false;
}

void ObjectHandler::PackObjectIntoVector(ObjectClass* in_ptr, std::vector<ObjectPackage>& in_target_vector) {
	ObjectPackage package;

	//Retrieve relevant data from object
	package.id = in_ptr->GetObjectID();
	package.model_matrix = in_ptr->GetModelMatrix();

	//Add package to the given vector reference
	in_target_vector.push_back(package);
}

void ObjectHandler::PackObjectVectorIntoVector(std::vector<ObjectClass*>& in_ptr_vector, std::vector<ObjectPackage>& in_target_vector) {
	//Pack all objects in given vector into the given vector reference
	for (unsigned int i = 0; i < in_ptr_vector.size(); i++) {
		this->PackObjectIntoVector(in_ptr_vector.at(i), in_target_vector);
	}
}

//Public---------------------------------------------------

ObjectHandler::ObjectHandler() {
	this->player_ptr_ = NULL;
	this->physics_engine_ptr_ = NULL;
}

ObjectHandler::~ObjectHandler() {
	delete this->player_ptr_;
	this->ClearPtrVector(this->npc_ptr_vector_);
	this->ClearPtrVector(this->drop_ptr_vector_);

	delete this->physics_engine_ptr_;

}

void ObjectHandler::InitializeObjectHandler(std::vector<std::vector<float>>* map_height_list) {

	// z = ~90-95
	this->player_ptr_ = new ObjectClass(glm::vec3(100.0f, -400.0f, -95.0f), OBJECT_ID_PLAYER);
	this->player_ptr_->SetScale(10.0f);

	this->physics_engine_ptr_ = new PhysicsEngine(
		GRAVITATIONAL_ACCELERATION,
		OBJECT_MAX_VELOCITY,
		OBJECT_MIN_VELOCITY,
		OBJECT_DECCELERATION,
		map_height_list
	);

	
	//this->TestObjectHandler();		//NTS: Just for testing


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
	this->player_input_.use_ability = true;
}

void ObjectHandler::PlayerPickUp() {
	this->player_input_.pick_up = true;
}

std::vector<ObjectPackage> ObjectHandler::UpdateAndRetrieve(float in_deltatime) {

	
	std::vector<ObjectClass*> relevant_npcs_ptr_vector;		//Two vectors to hold the NPC:s and drops that are within the culling distance
	std::vector<ObjectClass*> relevant_drops_ptr_vector;
	std::vector<ObjectClass*> physical_objects_ptr_vector;	//A vector to hold everything that is affected by physics
	

	//Cull NPC:s
	relevant_npcs_ptr_vector = this->CullAndRetrieveObjectPtrs(this->npc_ptr_vector_);

	//Cull Drops
	relevant_drops_ptr_vector = this->CullAndRetrieveObjectPtrs(this->drop_ptr_vector_);

	//Fill physical vector with the player and all NPC:s
	physical_objects_ptr_vector.push_back(this->player_ptr_);
	physical_objects_ptr_vector.insert(
		physical_objects_ptr_vector.end(),
		relevant_npcs_ptr_vector.begin(),
		relevant_npcs_ptr_vector.end()
	);

	//Take input from player (i.e. set velocity, attack flags, etc)
	DeterminePlayerAction();
	
	//Apply physics such as moving or falling
	this->physics_engine_ptr_->ApplyPhysics(in_deltatime, physical_objects_ptr_vector);

	//DetermineNPCAction(/*vector.at(i)*/);

	//ResolvePlayerAction();
	
	//ResolveNPCAction(/*vector.at(i)*/);

	//ResolveDropBehaviour(in_drop);

	//Reset the inputs
	this->ClearPlayerInput();

	//Put the things that are to be drawn onto screen in a package struct and return it
	std::vector<ObjectPackage> package_vector;
	
	this->PackObjectIntoVector(this->player_ptr_, package_vector);
	this->PackObjectVectorIntoVector(relevant_npcs_ptr_vector, package_vector);
	this->PackObjectVectorIntoVector(relevant_drops_ptr_vector, package_vector);

	return package_vector;
}

glm::vec3 ObjectHandler::GetPlayerPos() {
	return this->player_ptr_->GetPosition();
}

void ObjectHandler::TestObjectHandler() {
	std::cout << "Test of ObjectHandler started" << std::endl;

	glm::vec3 best_pos = glm::vec3(0.0f, 0.0f, 0.0f);

	std::cout << "A:	Creating three ObjectClass:es in npc_vector_" << std::endl;
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));

	std::cout << "B:	Moving second entry (index 1) to position (3000, 3000)" << std::endl;
	this->npc_ptr_vector_.at(1)->SetPosition(3000, 3000);
	glm::vec3 test_pos;
	for (unsigned int i = 0; i < this->npc_ptr_vector_.size(); i++) {
		test_pos = this->npc_ptr_vector_.at(i)->GetPosition();
		std::cout << "	ObjectClass [" << i << "]: (" << test_pos.x << "," << test_pos.y << "," << test_pos.z << ")" << std::endl;
	}

	std::cout << "C:	Creating culled vector" << std::endl;
	std::vector<ObjectClass*> test_vector = this->CullAndRetrieveObjectPtrs(this->npc_ptr_vector_);
	std::cout << "	npc_vector_ length: " << this->npc_ptr_vector_.size() << std::endl;
	std::cout << "	test_vector length: " << test_vector.size() << std::endl;

	std::cout << "D:	Changing the position of first entry to (-3000, -3000) via pointer in test vector" << std::endl;
	test_vector.at(0)->SetPosition(-3000, -3000);
	for (unsigned int i = 0; i < this->npc_ptr_vector_.size(); i++) {
		test_pos = this->npc_ptr_vector_.at(i)->GetPosition();
		std::cout << "	ObjectClass [" << i << "]: (" << test_pos.x << "," << test_pos.y << "," << test_pos.z << ")" << std::endl;
	}

	std::cout << "E:	Creating new culled vector" << std::endl;
	test_vector = this->CullAndRetrieveObjectPtrs(this->npc_ptr_vector_);
	std::cout << "	npc_vector_ length: " << this->npc_ptr_vector_.size() << std::endl;
	std::cout << "	test_vector length: " << test_vector.size() << std::endl;


	std::cout << "F:	Remove the last entry in the culled vector from the npc_ptr_vector_" << std::endl;
	this->RemoveObject(test_vector.at(0), this->npc_ptr_vector_);
	for (unsigned int i = 0; i < this->npc_ptr_vector_.size(); i++) {
		test_pos = this->npc_ptr_vector_.at(i)->GetPosition();
		std::cout << "	ObjectClass [" << i << "]: (" << test_pos.x << "," << test_pos.y << "," << test_pos.z << ")" << std::endl;
	}
	std::cout << "	Word of warning, the culled vector still holds 1 element: a pointer leading to trash" << std::endl;
	std::cout << "	npc_vector_ length: " << this->npc_ptr_vector_.size() << std::endl;
	std::cout << "	test_vector length: " << test_vector.size() << std::endl;

	std::cout << "G:	Clear npc_ptr_vector from its elements" << std::endl;
	this->ClearPtrVector(this->npc_ptr_vector_);
	std::cout << "	npc_vector_ length: " << this->npc_ptr_vector_.size() << std::endl;

	std::cout << "H:	Testing the UpdateAndRetrive() early function" << std::endl;
	std::cout << "	Add 2 objects to npc vector and 2 to drop vector" << std::endl;
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
	this->drop_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
	this->drop_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
	std::cout << "	npc_vector_ length: " << this->npc_ptr_vector_.size() << std::endl;
	std::cout << "	drop_vector_ length: " << this->drop_ptr_vector_.size() << std::endl;
	std::cout << "	Move one drop to (3000, 3000)" << std::endl;
	this->drop_ptr_vector_.at(0)->SetPosition(3000, 3000);
	std::cout << "	Call UpdateAndRetrieve and check returned vector length (should be 4: One Player, Two NPCs, One Drop)" << std::endl;
	std::vector<ObjectPackage> pckg_vector = this->UpdateAndRetrieve(1.0f);
	std::cout << "	pckg_vec length: " << pckg_vector.size() << std::endl;
}