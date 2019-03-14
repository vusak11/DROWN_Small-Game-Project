#include "object_handler.h"
#include "Character/NPCs/NPC_ghost.h"

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
		if (this->DistanceBetween(this->player_ptr_, in_object_vector.at(i)) < GlobalSettings::Access()->ValueOf("OBJECT_CULLING_DISTANCE")) {
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

void ObjectHandler::DeterminePlayerAction(
	const float& in_deltatime,
	std::vector<ObjectClass*>& in_relevant_npcs_ptr_vector,
	std::vector<ObjectClass*>& in_relevant_drops_ptr_vector
) {

	//Update the player's status (such as cooldowns)
	this->player_ptr_->UpdateStatus(in_deltatime);
	player_ptr_->CalculateAnimationState(in_deltatime);
	static float time = 0;
	time += in_deltatime;

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
		if (time > player_ptr_->GetWeapon()->GetCooldown())
		{
			time = 0;
			this->ResolvePlayerAttack(in_relevant_npcs_ptr_vector);
		}
		
	}
	//If input is to pick up
	if (this->player_input_.pick_up) {
		this->ResolvePlayerPickUp(in_relevant_drops_ptr_vector);
	}

	// Uppdate the listeners postion for the sound
	//glm::vec3 tmp_pos = player_ptr_->GetPosition();
	//sf::Listener::setPosition(tmp_pos.x, tmp_pos.y, tmp_pos.z);
	//sf::Listener::setDirection(player_ptr_->GetFacingDirection(), 0.0f, 1.0f);
	//sf::Listener::setUpVector(0.f, 1.f, 0.f);
	
	
	

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
		//Check if the ptr is an ability
		bool same_ability = false;
		drop_ptr = dynamic_cast<AbilitiesDrop*>(in_relevant_drops_ptr_vector.at(index));
		//Swap abilities
		if (drop_ptr != NULL) {
			AbilityID old_ability = player_ptr_->GetAbilityID();
			bool ability_swapped = player_ptr_->SwapAbilities(in_relevant_drops_ptr_vector.at(index)->GetObjectID());

			if (ability_swapped) {
				if (old_ability == ABILITY_DOUBLE_JUMP)
					this->drop_ptr_vector_.push_back(
						new DoubleJumpDrop(in_relevant_drops_ptr_vector.at(index)->GetPosition()));
				else if (old_ability == ABILITY_DASH)
					this->drop_ptr_vector_.push_back(
						new DashDrop(in_relevant_drops_ptr_vector.at(index)->GetPosition()));
				this->drop_ptr_vector_.back()->SetScale(3.0f);
				this->drop_ptr_vector_.back()->SetOffsets(3.0f, 3.0f);
			}
			else
				same_ability = true;
		}
		
		if (!same_ability) { //OBS! This case is needed for swapping abilities otherwise we will have to duplicate content
			//Delete the object and remove the pointer from the object handler's drop vector
			this->RemoveObject(in_relevant_drops_ptr_vector.at(index), this->drop_ptr_vector_);
			//Then remove the entry from the list of relevant drops
			in_relevant_drops_ptr_vector.erase(in_relevant_drops_ptr_vector.begin() + index);
		}
	}
}

void ObjectHandler::ResolvePlayerAttack(std::vector<ObjectClass*>& in_relevant_npcs_ptr_vector) {

	std::vector<int> index_of_the_dead;
	Character* character_ptr = NULL;
	int outcome = -1;
	int sound_index = -1;

	// First check if boss is hit
	if (boss_ptr_)
	{
		sound_index = ResolveBossAttack();
	}

	//Loop over all relevant npcs
	for (unsigned int i = 0; i < in_relevant_npcs_ptr_vector.size(); i++) {
		//Typecast a ptr in the vector to the character type
		character_ptr = dynamic_cast<Character*>(in_relevant_npcs_ptr_vector.at(i));
		if (character_ptr != NULL) {
			//Send in a npc and check if the player hits it with the attack
			//If the unit dies save its index
			outcome = this->player_ptr_->UseWeapon(*character_ptr);
			if (0 == outcome && sound_index != 1)
			{
				sound_index = 0;
			}
			if (1 == outcome) {
				index_of_the_dead.push_back(i);
				sound_index = 1;
			}
		}
	}

	this->player_ptr_->PlaySound(sound_index);

	//Lastly remove enemies on position indicated by the index vector
	//We need to track the offset  of how many thing we have deleted
	//to be able to access the right index in the relevant npcs vector
	int index;
	int offset = 0;
	for (unsigned int i = 0; i < index_of_the_dead.size(); i++) {

		//Pick index from vector
		index = index_of_the_dead.at(i);

		//Delete the object and remove the pointer from the object handler's npc vector
		this->RemoveObject(in_relevant_npcs_ptr_vector.at(index), this->npc_ptr_vector_);

		//Then remove the entry from the list of relevant drops
		in_relevant_npcs_ptr_vector.erase(in_relevant_npcs_ptr_vector.begin() + (index - offset));

		//Increase offset
		offset++;
	}


}

int ObjectHandler::ResolveBossAttack() {
	int sound_index = -1;

	if (player_ptr_->UseWeapon(*boss_ptr_) != -1)
	{
		sound_index = 0;
	}

	//std::vector<NPCGhost*>* npc_vector = &boss_ptr_->GetBossNPCVector();
	//Character* character_ptr = NULL;
	//int outcome = -1;

	//for (unsigned int i = 0; i < npc_vector->size(); i++) {
	//	//Typecast a ptr in the vector to the character type
	//	character_ptr = dynamic_cast<Character*>(npc_vector->at(0));
	//	if (character_ptr != NULL) {
	//		outcome = this->player_ptr_->UseWeapon(*character_ptr);
	//		if (0 == outcome && sound_index != 1)
	//		{
	//			sound_index = 0;
	//		}
	//		if (1 == outcome) {
	//			sound_index = 1;
	//		}
	//	}
	//}


	return sound_index;
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
	in_npc->ExecuteAI(in_deltatime, player_ptr_->GetPosition());
	if (in_npc->CheckCollision(player_ptr_->GetPoints())) {
		in_npc->Attack(*player_ptr_);
	}
}

void ObjectHandler::ProcessDrops(const float& in_deltatime, std::vector<ObjectClass*>& in_drops_ptr_vector) {
	//For every entry, turn it into a Drop pointer
	//and then call the Rotate function
	Drop* drop_ptr = NULL;

	for (unsigned int i = 0; i < in_drops_ptr_vector.size(); i++) {
		//Do dynamic cast
		drop_ptr = dynamic_cast<Drop*>(in_drops_ptr_vector.at(i));

		//If it succeded, call the rotate function
		if (drop_ptr != NULL) {
			drop_ptr->SpinDrop(in_deltatime);
		}
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
//
//void ObjectHandler::PackObjectVectorIntoVector(std::vector<NPCGhost*>& in_ptr_vector, std::vector<ObjectPackage>& in_target_vector) {
//	//Pack all objects in given vector into the given vector reference
//	for (unsigned int i = 0; i < in_ptr_vector.size(); i++) {
//		this->PackObjectIntoVector(in_ptr_vector.at(i), in_target_vector);
//	}
//}

//Public---------------------------------------------------

ObjectHandler::ObjectHandler() {
	this->player_ptr_ = NULL;
	this->boss_ptr_ = NULL;
	this->physics_engine_ptr_ = NULL;
}

ObjectHandler::~ObjectHandler() {
	delete this->player_ptr_;
	this->ClearPtrVector(this->npc_ptr_vector_);
	this->ClearPtrVector(this->drop_ptr_vector_);
	delete this->boss_ptr_;

	delete this->physics_engine_ptr_;
	
}

void ObjectHandler::InitializeObjectHandler(std::vector<std::vector<float>>* map_height_list, MetaData* meta_data) {

	//Create player
	//Assign spawn position randomly via meta data
	this->player_ptr_ = new PlayerCharacter(glm::vec3(meta_data->GetSpawnPointCoords(), 3.0f));
	//this->player_ptr_ = new PlayerCharacter(glm::vec3(564.0f, -220.0f, 5.0f));
	this->player_ptr_->SetScale(2.0f);
	this->player_ptr_->SetOffsets(2, 2);
	this->player_ptr_->LoadPlayerSounds();
	
	glm::vec3 drop_pos = this->player_ptr_->GetPosition();
	//TEMP
	drop_pos.x += 10.0f;
	this->drop_ptr_vector_.push_back(new DoubleJumpDrop(drop_pos));
	this->drop_ptr_vector_.back()->SetScale(3.0f);
	this->drop_ptr_vector_.back()->SetOffsets(3.0f, 3.0f);

	// Create NPCs and spawn them on every light source
	for (int i = 2; i < meta_data->GetLightPositions().size(); i++) {
		if (sqrt(pow((meta_data->GetLightPositions()[i].x - meta_data->GetSpawnPointCoords().x), 2) + pow((meta_data->GetLightPositions()[i].y - meta_data->GetSpawnPointCoords().y), 2)) > 50) {
			// Different NPC's depending on where they spawn
			if (meta_data->GetZone(meta_data->GetLightPositions()[i]) == "DEF") {
				this->npc_ptr_vector_.push_back(new NPCRunner(glm::vec3(meta_data->GetLightPositions()[i], 5.0f), OBJECT_ID_DUMMY));
			}
			else if (meta_data->GetZone(meta_data->GetLightPositions()[i]) == "RED") {
				this->npc_ptr_vector_.push_back(new NPCRunner(glm::vec3(meta_data->GetLightPositions()[i], 5.0f), OBJECT_ID_FIRE_AI));
			}
			else if (meta_data->GetZone(meta_data->GetLightPositions()[i]) == "GRE") {
				this->npc_ptr_vector_.push_back(new NPCRunner(glm::vec3(meta_data->GetLightPositions()[i], 5.0f), OBJECT_ID_WOOD_AI));
			}
			else if (meta_data->GetZone(meta_data->GetLightPositions()[i]) == "BLU") {
				this->npc_ptr_vector_.push_back(new NPCRunner(glm::vec3(meta_data->GetLightPositions()[i], 5.0f), OBJECT_ID_ICE_AI));
			}
			float scale = GlobalSettings::Access()->ValueOf("NPC_RUNNER_SCALE");
			this->npc_ptr_vector_.back()->SetScale(scale);
			this->npc_ptr_vector_.back()->SetOffsets(scale, scale);
		}
	}
	this->nr_of_runners_ = this->npc_ptr_vector_.size();

	// Spawn keys
	for (int i = 0; i < 3; i++) {
		this->drop_ptr_vector_.push_back(new KeyDrop(glm::vec3(meta_data->GetDoorKeyCoords()[i], 0.0f)));
		this->drop_ptr_vector_.back()->SetScale(3.0f);
		this->drop_ptr_vector_.back()->SetOffsets(3.0f, 3.0f);
	}
	// Spawn boss door
	this->drop_ptr_vector_.push_back(new BossDoor(glm::vec3(meta_data->GetBossDoorCoords(), 0.0f)));
	this->drop_ptr_vector_.back()->SetScale(3.0f);
	this->drop_ptr_vector_.back()->SetOffsets(3.0f, 3.0f);

	// Apply physics
	this->physics_engine_ptr_ = new PhysicsEngine(map_height_list);


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
	
	//--------------------------------------------------------
	//-------------Calculate & Process Objects----------------
	//--------------------------------------------------------

	//Cull NPC:s
	relevant_npcs_ptr_vector = this->CullAndRetrieveObjectPtrs(this->npc_ptr_vector_);

	//Cull Drops
	relevant_drops_ptr_vector = this->CullAndRetrieveObjectPtrs(this->drop_ptr_vector_);

	//Take input from player (i.e. set velocity, attack flags, etc)
	this->DeterminePlayerAction(in_deltatime, relevant_npcs_ptr_vector, relevant_drops_ptr_vector);
	
	//Go through all relevant NPCs and call their AI functions
	this->ProcessNPCs(in_deltatime, relevant_npcs_ptr_vector);

	//Go through all relevant drops and call their behaviour functions
	//this->ProcessDrops(in_deltatime, relevant_drops_ptr_vector);

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

	// if boss exists, process all boss stuff
	if (boss_ptr_) {
		boss_ptr_->ExecuteAI(in_deltatime, player_ptr_->GetPosition());
		boss_ptr_->UpdateBossDamageToPlayer(in_deltatime, player_ptr_);
		this->PackObjectIntoVector(this->boss_ptr_, package_vector);
		std::vector<ObjectClass*> temp_boss_list = boss_ptr_->GetBossObjectVector();
		this->PackObjectVectorIntoVector(temp_boss_list, package_vector);

		/*std::vector<NPCGhost*> temp_boss_npc_list = boss_ptr_->GetBossNPCVector();
		this->PackObjectVectorIntoVector(temp_boss_npc_list, package_vector);*/

		DetermineBossAction();
	}
	

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

void ObjectHandler::SetPlayerXYZPosForBoss()
{
	this->player_ptr_->SetVelocityVec({ 0,0,0 });

	this->player_ptr_->SetPosition(
		GlobalSettings::Access()->ValueOf("DROP_BOSS_DOOR_DESTINATION_X"),
		GlobalSettings::Access()->ValueOf("DROP_BOSS_DOOR_DESTINATION_Y"),
		7
	);
}

void ObjectHandler::SpawnBoss() {
	
	boss_ptr_ = new NPCBoss(glm::vec3(160, -1110, 10));
	boss_ptr_->SetPosition(160, -1110, 10);
	std::cout << "Spawned boss" << std::endl;
}

void ObjectHandler::DetermineBossAction() {

	if (boss_ptr_->actions_.spawn_mobs)
	{
		
		if (boss_ptr_->GetBossStage() == BossStage::STAGE_2)
		{
			this->npc_ptr_vector_.push_back(new NPCRunner(glm::vec3(100, -1180, 5.0f), OBJECT_ID_FIRE_AI));
			this->npc_ptr_vector_.back()->SetScale(2);
			this->npc_ptr_vector_.back()->SetOffsets(2, 2);
			NPCRunner* temp_npc_ptr = dynamic_cast<NPCRunner*>(this->npc_ptr_vector_.back());
			temp_npc_ptr->SetAggroRange(200);

			this->npc_ptr_vector_.push_back(new NPCRunner(glm::vec3(220, -1180, 5.0f), OBJECT_ID_FIRE_AI));
			this->npc_ptr_vector_.back()->SetScale(2);
			this->npc_ptr_vector_.back()->SetOffsets(2, 2);
			temp_npc_ptr = dynamic_cast<NPCRunner*>(this->npc_ptr_vector_.back());
			temp_npc_ptr->SetAggroRange(200);
		}
		
		if (boss_ptr_->GetBossStage() == BossStage::STAGE_3)
		{
			int rand_x_pos = rand() % 220 + 100;
			this->npc_ptr_vector_.push_back(new NPCRunner(glm::vec3(rand_x_pos, -1100, 5.0f), OBJECT_ID_FIRE_AI));
			this->npc_ptr_vector_.back()->SetScale(1);
			this->npc_ptr_vector_.back()->SetOffsets(1, 1);
			NPCRunner* temp_npc_ptr = dynamic_cast<NPCRunner*>(this->npc_ptr_vector_.back());
			temp_npc_ptr->SetAggroRange(200);
			temp_npc_ptr->SetJumpVelocity(300);
			temp_npc_ptr->SetAggroVelocity(3000);
		}

		boss_ptr_->actions_.spawn_mobs = false;
	}
	
	if (boss_ptr_->actions_.spawn_ghost)
	{
		int position_index = rand() % 3;
		switch (position_index)
		{
		case 0:
			this->npc_ptr_vector_.push_back(new NPCGhost(glm::vec3(160, -1050, 5.0f), OBJECT_ID_NULL));
			break;
		case 1:
			this->npc_ptr_vector_.push_back(new NPCGhost(glm::vec3(20, -1180, 5.0f), OBJECT_ID_NULL));
			break;
		case 2:
			this->npc_ptr_vector_.push_back(new NPCGhost(glm::vec3(300, -1180, 5.0f), OBJECT_ID_NULL));
			break;
		default:
			break;
		}
		
		boss_ptr_->actions_.spawn_ghost = false;
	}

	if (boss_ptr_->actions_.spawn_jombo)
	{
		this->npc_ptr_vector_.push_back(new NPCRunner(glm::vec3(160, -1100, 5.0f), OBJECT_ID_WOOD_AI));
		this->npc_ptr_vector_.back()->SetScale(4);
		this->npc_ptr_vector_.back()->SetOffsets(4, 4);
		NPCRunner* temp_npc_ptr = dynamic_cast<NPCRunner*>(this->npc_ptr_vector_.back());
		temp_npc_ptr->SetAggroRange(300);
		temp_npc_ptr->SetJumpVelocity(150);
		temp_npc_ptr->SetAggroVelocity(1000);
		temp_npc_ptr->SetMaxHealth(120);
		temp_npc_ptr->SetCurrentHealth(120);
	}

	

}

