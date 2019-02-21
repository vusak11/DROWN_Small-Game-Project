#ifndef OBJECT_OBJECT_HANDLER_H_
#define OBJECT_OBJECT_HANDLER_H_

#include <vector>
#include <functional>		//Allows function pointers and lambdas
							//Ref. for how it works: https://en.cppreference.com/w/cpp/utility/functional/function

#include "../globals.h"

#include "object_class.h"
#include "Character/character.h"
#include "Character/player_character.h"
#include "Character/npc.h"


#include "Physics/physics_engine.h"

#include <iostream>			//Included for output in test function.

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

	PlayerInput player_input_;

	PhysicsEngine* physics_engine_ptr_;

	//std::vector<std::function<void()>> player_action_queue_;		//NTS: Not at home in ObjectHandler?
	

	//Functions-----------------------------------------------
	bool ClearPtrVector(std::vector<ObjectClass*>& in_vec);
	
	bool RemoveObject(const ObjectClass* in_object_ptr, std::vector<ObjectClass*>& in_object_ptr_vector);			//Takes an object, scans the given vector for it, removes it if found, returns true if success 
	
	std::vector<ObjectClass*> CullAndRetrieveObjectPtrs(const std::vector<ObjectClass*>& in_object_vector) const;	//Scans given vector and returns a vector of references to objects close enough to the player
				
	float DistanceBetween(const ObjectClass* in_object_a, const ObjectClass* in_object_b) const;			//Returns the distance between the two given objects

	void DeterminePlayerAction(const float& in_deltatime);						//Read player_input_ and determine legal actions, such as changes to velocity or if we can attack this frame
	void DetermineNPCAction(ObjectClass* in_npc);		//Call the AI of the npc object to see what the npc does, then determine legal actions

	void ResolvePlayerAction();							//Move player, apply hitboxes, etc.
	void ResolveNPCAction(ObjectClass* in_npc);			//Move npc, apply hitboxes, etc.

	void ResolveDropBehaviour(ObjectClass* in_drop);	//Rotates drop, counts towards its despawn, etc.

	void ClearPlayerInput();							//Sets all values in player_input_ to false. Should be called at the end of each Update()

	void PackObjectIntoVector(ObjectClass* in_ptr, std::vector<ObjectPackage>& in_target_vector);
	void PackObjectVectorIntoVector(std::vector<ObjectClass*>& in_ptr_vector, std::vector<ObjectPackage>& in_target_vector);

public:
	ObjectHandler();
	~ObjectHandler();

	// Takes map data to send to physics engine.
	void InitializeObjectHandler(std::vector<std::vector<float>>* map_height_list);		//Function creates player object, loads in enemies for the zones etc. Call from InitiateGame();

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

	glm::vec3 GetPlayerPos();

	//Test functions------------------------------------------
	void TestObjectHandler();

};

#endif // !OBJECT_OBJECT_HANDLER_H_
