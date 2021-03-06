#ifndef OBJECT_OBJECT_HANDLER_H_
#define OBJECT_OBJECT_HANDLER_H_

#include <vector>
#include <iostream>			//Included for debug output

#include "object_class.h"

#include "Character/character.h"
#include "Character/player_character.h"
#include "Character/npc.h"
#include "Character/NPCs/NPC_runner.h"
#include "Character/NPCs/NPC_boss.h"

#include "../Map/metadata.h"
#include "Drop/Drop.h"

#include "Physics/physics_engine.h"
#include "randomizer.h"

class ObjectHandler {
private:

	//Structs-------------------------------------------------
	struct PlayerInput {		//PlayerInput is made to help track button presses
		bool left = false;
		bool right = false;
		bool jump = false;
		bool attack = false;
		bool use_ability = false;
		bool pick_up = false;
	};

	//Variables-----------------------------------------------
	PlayerCharacter* player_ptr_;
	std::vector<ObjectClass*> npc_ptr_vector_;		//All enemies
	std::vector<ObjectClass*> drop_ptr_vector_;		//Things dropped on the ground (e.g. power-ups, health)
	NPCBoss* boss_ptr_;

	int nr_of_runners_;

	PlayerInput player_input_;

	PhysicsEngine* physics_engine_ptr_;
	
	Randomizer* randomizer_ptr_;
	float enemy_drop_rate_;
	float chest_drop_rate_;

	//Functions-----------------------------------------------
	bool ClearPtrVector(std::vector<ObjectClass*>& in_vec);
	
	bool RemoveObject(const ObjectClass* in_object_ptr, std::vector<ObjectClass*>& in_object_ptr_vector);			//Takes an object, scans the given vector for it, removes it if found, returns true if success 
	
	std::vector<ObjectClass*> CullAndRetrieveObjectPtrs(const std::vector<ObjectClass*>& in_object_vector) const;	//Scans given vector and returns a vector of references to objects close enough to the player
				
	float DistanceBetween(const ObjectClass* in_object_a, const ObjectClass* in_object_b) const;					//Returns the distance between the two given objects

	void DeterminePlayerAction(
		const float& in_deltatime,
		std::vector<ObjectClass*>& in_relevant_npcs_ptr_vector,
		std::vector<ObjectClass*>& in_relevant_drops_ptr_vector
	);
	void ResolvePlayerPickUp(std::vector<ObjectClass*>& in_relevant_drops_ptr_vector);
	void ResolvePlayerAttack(std::vector<ObjectClass*>& in_relevant_npcs_ptr_vector);
	void ResolveRandomDropSpawn(glm::vec3 in_pos, float in_drop_rate);
	int ResolveBossAttack();

	void ProcessNPCs(const float& in_deltatime, std::vector<ObjectClass*>& in_npcs_ptr_vector);	//Call AI functions for NPCs in vector
	void DetermineNPCAction(const float& in_deltatime, NPC* in_npc);							//Call the AI of the npc object to see what the npc does, then determine legal actions

	void ProcessDrops(const float& in_deltatime, std::vector<ObjectClass*>& in_drops_ptr_vector);	//Rotate drops

	void RemoveDeadNPCs(std::vector<ObjectClass*>& in_relevant_npcs_ptr_vector);

	void ClearPlayerInput();							//Sets all values in player_input_ to false. Should be called at the end of each Update()

	void PackObjectIntoVector(ObjectClass* in_ptr, std::vector<ObjectPackage>& in_target_vector);
	void PackObjectVectorIntoVector(std::vector<ObjectClass*>& in_ptr_vector, std::vector<ObjectPackage>& in_target_vector);

public:
	ObjectHandler();
	~ObjectHandler();

	// Takes map data to send to physics engine.
	void InitializeObjectHandler(
		std::vector<std::vector<float>>* map_height_list,
		MetaData* meta_data);		//Function creates player object, loads in enemies for the zones etc. Call from InitiateGame();

	//Functions for controlling the player
	//Call these functions when pressing buttons
	void PlayerMoveLeft();
	void PlayerMoveRight();
	void PlayerJump();
	void PlayerAttack();
	void PlayerUseAbility();
	void PlayerPickUp();

	//Called once per loop to update object positions
	//	- Read player inputs and determine effects
	//	- Call AI functions and decide behaviour
	//	- Execute NPC actions and determine effects
	//	- Call physics to determine new positions (<- Should be called at every step when a creature moves instead?)
	std::vector<ObjectPackage> UpdateAndRetrieve(float in_deltatime);

	PlayerInfoPackage RetrievePlayerInfoPackage();

	bool PlayerInBossRoom();
	void SetPlayerXYZPosForBoss();	// Adds a few values to z to move the player more into the room.
	void SpawnBoss();
	void DetermineBossAction();	// From wich flags are true in the boss, do things
	bool GetBossAttackState();	// On for lighting the warning light at the boss.
	bool IsBossDead();

};

#endif // !OBJECT_OBJECT_HANDLER_H_
