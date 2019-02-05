#ifndef OBJECT_OBJECT_HANDLER_H_
#define OBJECT_OBJECT_HANDLER_H_

#include <vector>
#include <functional>		//Allows function pointers and lambdas
							//Ref. for how it works: https://en.cppreference.com/w/cpp/utility/functional/function

#include "object_class.h"
#include "../Globals.h"

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
	ObjectClass player_object_;				//Player
	std::vector<ObjectClass> npc_vector_;	//All enemies
	std::vector<ObjectClass> drop_vector_;	//Things dropped on the ground (e.g. power-ups, health)

	PlayerInput player_input_;

	//std::vector<std::function<void()>> player_action_queue_;		//NTS: Not at home in ObjectHandler?
	

	//Functions-----------------------------------------------
	std::vector<ObjectClass*> CullAndRetrieveObjects(std::vector<ObjectClass>& in_object_vector);	//Scans given vector and returns a vector of references to objects close enough to the player
				
	float DistanceBetween(const ObjectClass& in_object_a, const ObjectClass& in_object_b);			//Returns the distance between the two given objects

	void DeterminePlayerAction();						//Read player_input_ and determine legal actions, such as changes to velocity or if we can attack this frame
	void DetermineNPCAction(ObjectClass in_npc);		//Call the AI of the npc object to see what the npc does, then determine legal actions

	void ResolvePlayerAction();							//Move player, apply hitboxes, etc.
	void ResolveNPCAction(ObjectClass);					//Move npc, apply hitboxes, etc.

	void ClearPlayerInput();							//Sets all values in player_input_ to false. Should be called at the end of each Update()

public:
	ObjectHandler();
	~ObjectHandler();

	//Functions for controlling the player
	//Call these functions from eventhandler
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
	void Update();

	//Test functions------------------------------------------
	void TestObjectHandler() {
		std::cout << "Test of ObjectHandler started" << std::endl;

		std::cout << "A:	Creating three ObjectClass:es in npc_vector_" << std::endl;
		this->npc_vector_.push_back(ObjectClass());
		this->npc_vector_.push_back(ObjectClass());
		this->npc_vector_.push_back(ObjectClass());

		std::cout << "B:	Moving second entry (index 1) to position (3000, 3000)" << std::endl;
		this->npc_vector_.at(1).SetPosition(3000, 3000);
		glm::vec3 test_pos;
		for (unsigned int i = 0; i < this->npc_vector_.size(); i++) {
			test_pos = this->npc_vector_.at(i).GetPosition();
			std::cout << "ObjectClass [" << i << "]: (" << test_pos.x << "," << test_pos.y << "," << test_pos.z << ")" << std::endl;
		}

		std::cout << "C:	Creating culled vector" << std::endl;
		std::vector<ObjectClass*> test_vector = this->CullAndRetrieveObjects(this->npc_vector_);
		std::cout << "npc_vector_ length: " << this->npc_vector_.size() << std::endl;
		std::cout << "test_vector length: " << test_vector.size() << std::endl;

		std::cout << "D:	Changing the position of first entry to (-3000, -3000) via pointer in test vector" << std::endl;
		test_vector.at(0)->SetPosition(-3000, -3000);
		for (unsigned int i = 0; i < this->npc_vector_.size(); i++) {
			test_pos = this->npc_vector_.at(i).GetPosition();
			std::cout << "ObjectClass [" << i << "]: (" << test_pos.x << "," << test_pos.y << "," << test_pos.z << ")" << std::endl;
		}

		std::cout << "E:	Creating new culled vector" << std::endl;
		test_vector = this->CullAndRetrieveObjects(this->npc_vector_);
		std::cout << "npc_vector_ length: " << this->npc_vector_.size() << std::endl;
		std::cout << "test_vector length: " << test_vector.size() << std::endl;
	}

};

#endif // !OBJECT_OBJECT_HANDLER_H_
