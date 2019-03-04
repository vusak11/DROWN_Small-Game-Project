#include "object_handler.h"

//Private--------------------------------------------------
bool ObjectHandler::ClearPtrVector(std::vector<ObjectClass*>& in_vec) {
	//While vector is not empty
	while (!in_vec.empty()) {
		//Delete the first object in vector
		delete in_vec.at(0);
		//then erase the first entry
		in_vec.erase(in_vec.begin() + 0);
	}

	return in_vec.empty();
}

bool ObjectHandler::RemoveObject(const ObjectClass* in_object_ptr, std::vector<ObjectClass*>& in_object_ptr_vector) {
	
	//WARNING: This function deletes the given object on a success
	//If a pointer has been saved externally it will lead to a trash location
	//Taking a reference to the pointer and setting it to NULL will not save
	//the situation since the pointer might have been copied outside

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
	//Returns distance between two objects on the x-y-plane

	//Get the 2D position for object a
	glm::vec3 temp_vec = in_object_a->GetPosition();
	glm::vec2 pos2_a = glm::vec2(temp_vec.x, temp_vec.y);
	
	//Get the 2D position for object b
	temp_vec = in_object_b->GetPosition();
	glm::vec2 pos2_b = glm::vec2(temp_vec.x, temp_vec.y);

	//Return the distance between them
	return glm::distance(pos2_a, pos2_b);
}

void ObjectHandler::DeterminePlayerAction(const float& in_deltatime, std::vector<ObjectClass*>& in_relevant_drops_ptr_vector) {

	//Update the player's status (such as cooldowns)
	this->player_ptr_->UpdateStatus(in_deltatime);

	//Determine player movement on the x-axis
	if (this->player_input_.left) {
		this->player_ptr_->MoveLeft();
		this->player_ptr_->TurnLeft(in_deltatime);
	}
	if (this->player_input_.right) {
		this->player_ptr_->MoveRight();
		this->player_ptr_->TurnRight(in_deltatime);
	}
	//If input is jump
	if (this->player_input_.jump) {
		this->player_ptr_->Jump();
	}
	//If input is use ability
	if (this->player_input_.use_ability) {
		this->player_ptr_->UseAbility();
	}
	//If input is attack
	if (this->player_input_.attack) {
		
	}
	//If input is to pick up
	if (this->player_input_.pick_up) {
		this->ResolvePlayerPickUp(in_relevant_drops_ptr_vector);
	}
	
}

void ObjectHandler::ResolvePlayerPickUp(std::vector<ObjectClass*>& in_relevant_drops_ptr_vector) {
	bool triggered = false;
	int index = 0;
	Drop* drop_ptr = NULL;

	//Loop over all relevant drops
	for (unsigned int i = 0; !triggered && (i < in_relevant_drops_ptr_vector.size()); i++) {
		//Typecast a ptr in the vector to the drop type
		drop_ptr = dynamic_cast<Drop*>(in_relevant_drops_ptr_vector.at(i));
		if (drop_ptr != NULL) {
			//Check if the player touches any of the drops (loop breaks if so)
			triggered = drop_ptr->CheckCollision(*(this->player_ptr_));
		}
		//Save current index
		index = i;
	}

	//If we have triggered an event
	if (triggered) {
		//Delete the object and remove the pointer from the object handler's drop vector
		this->RemoveObject(in_relevant_drops_ptr_vector.at(index), this->drop_ptr_vector_);
		//Then remove the entry from the list of relevant drops
		in_relevant_drops_ptr_vector.erase(in_relevant_drops_ptr_vector.begin() + index);
	}
}

void ObjectHandler::ProcessNPCs(const float& in_deltatime, std::vector<ObjectClass*>& in_npcs_ptr_vector) {
	//For every entry, turn it into a NPC pointer
	//and then call the AI function
	NPC* npc_ptr = NULL;
	for (unsigned int i = 0; i < in_npcs_ptr_vector.size(); i++) {
		//Do dynamic cast
		npc_ptr = dynamic_cast<NPC*>(in_npcs_ptr_vector.at(i));

		//If it succeded, call DetermineNPCAction
		if (npc_ptr != NULL) {
			this->DetermineNPCAction(in_deltatime, npc_ptr);
		}
	}
}

void ObjectHandler::DetermineNPCAction(const float& in_deltatime, NPC* in_npc) {

	//TEMP
	in_npc->ExecuteAI(in_deltatime);
	//TEMP

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
	package.model_matrix = in_ptr->RetrieveModelMatrix();

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

void ObjectHandler::InitializeObjectHandler(std::vector<std::vector<float>>* map_height_list, std::vector<glm::vec2> door_key_position) {

	//Create player
	this->player_ptr_ = new PlayerCharacter(PLAYER_START_POS);
	this->player_ptr_->SetScale(2.0f);

	//TEMP
	//glm::vec3 npc_pos = PLAYER_START_POS;
	//npc_pos.x -= 70.0f;
	//this->npc_ptr_vector_.push_back(new NPC(npc_pos));
	//this->npc_ptr_vector_.at(0)->SetScale(3.0f);

	glm::vec3 drop_pos = PLAYER_START_POS;

	drop_pos.x -= 10.0f;
	this->drop_ptr_vector_.push_back(new HPRestoreDrop(drop_pos));
	this->drop_ptr_vector_.back()->SetScale(3.0f);
	
	drop_pos.x -= 10.0f;
	this->drop_ptr_vector_.push_back(new HPUpDrop(drop_pos));
	this->drop_ptr_vector_.back()->SetScale(3.0f);

	drop_pos.x -= 10.0f;
	this->drop_ptr_vector_.push_back(new AtkUpDrop(drop_pos));
	this->drop_ptr_vector_.back()->SetScale(3.0f);
	
	drop_pos.x -= 10.0f;
	this->drop_ptr_vector_.push_back(new KeyDrop(drop_pos));
	this->drop_ptr_vector_.back()->SetScale(3.0f);
	//TEMP

	this->physics_engine_ptr_ = new PhysicsEngine(map_height_list);

	//Door! 
	//this->drop_ptr_vector_.push_back(new Drop(glm::vec3(door_key_position[0], 3.0f))); 
	//this->drop_ptr_vector_.at(0)->SetPosition(door_key_position[0].x, door_key_position[0].y, 3.0f);
	//this->drop_ptr_vector_.at(0)->SetScale(5.0f);

	//Retrieve the newly random generated position of door and key (0 = door, 1 = key)
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

void ObjectHandler::PlayerTeleport() { // TEMPORARY !!!
	this->player_ptr_->SetPosition(90.0f, -1250.0f, 0.0f);
}

std::vector<ObjectPackage> ObjectHandler::UpdateAndRetrieve(float in_deltatime) {

	
	std::vector<ObjectClass*> relevant_npcs_ptr_vector;		//Two vectors to hold the NPC:s and drops that are within the culling distance
	std::vector<ObjectClass*> relevant_drops_ptr_vector;
	std::vector<ObjectClass*> physical_objects_ptr_vector;	//A vector to hold everything that is affected by physics
	
	//--------------------------------------------------------
	//-------------Calculate & Process Objects----------------
	//--------------------------------------------------------

	//Cull NPC:s
	relevant_npcs_ptr_vector = this->CullAndRetrieveObjectPtrs(this->npc_ptr_vector_);

	//Cull Drops
	relevant_drops_ptr_vector = this->CullAndRetrieveObjectPtrs(this->drop_ptr_vector_);

	//Take input from player (i.e. set velocity, attack flags, etc)
	this->DeterminePlayerAction(in_deltatime, relevant_drops_ptr_vector);
	
	//Go through all relevant NPCs and call their AI functions
	this->ProcessNPCs(in_deltatime, relevant_npcs_ptr_vector);


	//WIP----

	//ResolvePlayerAction();

	//ResolveNPCAction(/*vector.at(i)*/);

	//ResolveDropBehaviour(in_drop);

	//WIP----


	//--------------------------------------------------------
	//------------------Apply Physics-------------------------
	//--------------------------------------------------------

	//Fill physical vector with the player and all NPC:s
	physical_objects_ptr_vector.push_back(this->player_ptr_);
	physical_objects_ptr_vector.insert(
		physical_objects_ptr_vector.end(),
		relevant_npcs_ptr_vector.begin(),
		relevant_npcs_ptr_vector.end()
	);
	physical_objects_ptr_vector.insert(
		physical_objects_ptr_vector.end(),
		relevant_drops_ptr_vector.begin(),
		relevant_drops_ptr_vector.end()
	);

	//Apply physics such as moving or falling
	this->physics_engine_ptr_->ApplyPhysics(in_deltatime, physical_objects_ptr_vector);

	//--------------------------------------------------------
	//------------------Clean Up & Return---------------------
	//--------------------------------------------------------

	//Reset the inputs
	this->ClearPlayerInput();

	//Put the things that are to be drawn onto screen in a package struct and return it
	std::vector<ObjectPackage> package_vector;
	
	this->PackObjectIntoVector(this->player_ptr_, package_vector);
	this->PackObjectVectorIntoVector(relevant_npcs_ptr_vector, package_vector);
	this->PackObjectVectorIntoVector(relevant_drops_ptr_vector, package_vector);

	return package_vector;
}

PlayerInfoPackage ObjectHandler::RetrievePlayerInfoPackage() {
	PlayerInfoPackage ret_info;

	ret_info.position = this->player_ptr_->GetPosition();
	ret_info.max_hp = this->player_ptr_->GetMaxHealth();
	ret_info.current_hp = this->player_ptr_->GetCurrentHealth();
	ret_info.ability_id = this->player_ptr_->GetAbilityID();
	ret_info.weapon_id = this->player_ptr_->GetWeaponID();
	ret_info.num_of_keys = this->player_ptr_->GetNumOfKeys();

	return ret_info;
}

bool ObjectHandler::PlayerInBossRoom() {
	bool check = false;
	 if (this->player_ptr_->GetPosition().x > 44.0f && this->player_ptr_->GetPosition().x < 396.0f &&
		this->player_ptr_->GetPosition().y > -1265.0f && this->player_ptr_->GetPosition().y < -1060.0f)
		check = true;
	 return check;
}

void ObjectHandler::TestObjectHandler() {
	std::cout << "Test of ObjectHandler started" << std::endl;

	glm::vec3 best_pos = glm::vec3(0.0f, 0.0f, 0.0f);

	std::cout << "A:	Creating three ObjectClass:es in npc_vector_" << std::endl;
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_DUMMY));
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_DUMMY));
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_DUMMY));

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
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_DUMMY));
	this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_DUMMY));
	this->drop_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_DUMMY));
	this->drop_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_DUMMY));
	std::cout << "	npc_vector_ length: " << this->npc_ptr_vector_.size() << std::endl;
	std::cout << "	drop_vector_ length: " << this->drop_ptr_vector_.size() << std::endl;
	std::cout << "	Move one drop to (3000, 3000)" << std::endl;
	this->drop_ptr_vector_.at(0)->SetPosition(3000, 3000);
	std::cout << "	Call UpdateAndRetrieve and check returned vector length (should be 4: One Player, Two NPCs, One Drop)" << std::endl;
	std::vector<ObjectPackage> pckg_vector = this->UpdateAndRetrieve(1.0f);
	std::cout << "	pckg_vec length: " << pckg_vector.size() << std::endl;

	std::cout << "I:	Test throw functions in Character" << std::endl;
	try { this->player_ptr_->HealDamage(-3); }
	catch (std::invalid_argument a){ std::cout << a.what() << std::endl; }
	
	try { this->player_ptr_->TakeDamage(-17); }
	catch (std::invalid_argument a) { std::cout << a.what() << std::endl; }

	try { this->player_ptr_->SetAttackPower(0); }
	catch (std::invalid_argument a) { std::cout << a.what() << std::endl; }

	try { this->player_ptr_->SetCurrentHealth(-98); }
	catch (std::invalid_argument a) { std::cout << a.what() << std::endl; }

	try { this->player_ptr_->SetCurrentHealth(1000); }
	catch (std::invalid_argument a) { std::cout << a.what() << std::endl; }

	try { this->player_ptr_->SetMaxHealth(-1); }
	catch (std::invalid_argument a) { std::cout << a.what() << std::endl; }

}