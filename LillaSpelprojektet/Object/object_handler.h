#ifndef OBJECT_OBJECT_HANDLER_H_
#define OBJECT_OBJECT_HANDLER_H_

#include <vector>
#include <functional>		//Allows function pointers and lambdas
							//Ref. for how it works: https://en.cppreference.com/w/cpp/utility/functional/function

#include "object_class.h"
#include "../globals.h"

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
	ObjectClass* player_ptr_;						//Player
	std::vector<ObjectClass*> npc_ptr_vector_;		//All enemies
	std::vector<ObjectClass*> drop_ptr_vector_;		//Things dropped on the ground (e.g. power-ups, health)

	PlayerInput player_input_;

	//std::vector<std::function<void()>> player_action_queue_;		//NTS: Not at home in ObjectHandler?
	

	//Functions-----------------------------------------------
	bool ClearPtrVector(std::vector<ObjectClass*>& in_vec);
	
	bool RemoveObject(const ObjectClass* in_object_ptr, std::vector<ObjectClass*>& in_object_ptr_vector);			//Takes an object, scans the given vector for it, removes it if found, returns true if success 
	
	std::vector<ObjectClass*> CullAndRetrieveObjectPtrs(const std::vector<ObjectClass*>& in_object_vector) const;	//Scans given vector and returns a vector of references to objects close enough to the player
				
	float DistanceBetween(const ObjectClass* in_object_a, const ObjectClass* in_object_b) const;			//Returns the distance between the two given objects

	void DeterminePlayerAction();						//Read player_input_ and determine legal actions, such as changes to velocity or if we can attack this frame
	void DetermineNPCAction(ObjectClass* in_npc);		//Call the AI of the npc object to see what the npc does, then determine legal actions

	void ResolvePlayerAction();							//Move player, apply hitboxes, etc.
	void ResolveNPCAction(ObjectClass* in_npc);			//Move npc, apply hitboxes, etc.

	void ResolveDropBehaviour(ObjectClass* in_drop);		//Rotates drop, counts towards its despawn, etc.

	void ClearPlayerInput();							//Sets all values in player_input_ to false. Should be called at the end of each Update()

	void PackObjectIntoVector(ObjectClass* in_ptr, std::vector<ObjectPackage>& in_target_vector);
	void PackObjectVectorIntoVector(std::vector<ObjectClass*>& in_ptr_vector, std::vector<ObjectPackage>& in_target_vector);

public:
	ObjectHandler();
	~ObjectHandler();

	void InitializeObjectHandler();		//Function creates player object, loads in enemies for the zones etc. Call from InitiateGame();

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
	std::vector<ObjectPackage> UpdateAndRetrieve();

	//Test functions------------------------------------------
	void TestObjectHandler() {
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
		std::cout << "	Add 2 objects to npc vector and 2 to drop vector"  << std::endl;
		this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
		this->npc_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
		this->drop_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
		this->drop_ptr_vector_.push_back(new ObjectClass(best_pos, OBJECT_ID_JOHNNY_BRAVO));
		std::cout << "	npc_vector_ length: " << this->npc_ptr_vector_.size() << std::endl;
		std::cout << "	drop_vector_ length: " << this->drop_ptr_vector_.size() << std::endl;
		std::cout << "	Move one drop to (3000, 3000)" << std::endl;
		this->drop_ptr_vector_.at(0)->SetPosition(3000, 3000);
		std::cout << "	Call UpdateAndRetrieve and check returned vector length (should be 4: One Player, Two NPCs, One Drop)" << std::endl;
		std::vector<ObjectPackage> pckg_vector = this->UpdateAndRetrieve();
		std::cout << "	pckg_vec length: " << pckg_vector.size() << std::endl;
	}

};

#endif // !OBJECT_OBJECT_HANDLER_H_
