#ifndef OBJECT_OBJECT_HANDLER_H_
#define OBJECT_OBJECT_HANDLER_H_

#include <vector>
#include <functional>		//Allows function pointers and lambdas
							//Ref. for how it works: https://en.cppreference.com/w/cpp/utility/functional/function

#include "object_class.h"
#include "../Globals.h"

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
	std::vector<ObjectClass> CullAndRetrieveNPCs();		//Scans npc_vector_ and returns NPCs close enough to the player
	std::vector<ObjectClass> CullAndRetrieveDrops();	//Scans drop_vector_ and returns drops close enough to the player

	//float DistanceBetween(ObjectClass in_object_a, ObjectClass in_object_b);				//Returns the distance between the two given objects
	float DistanceBetween(const ObjectClass& in_object_a, const ObjectClass& in_object_b);	//Alternate function using constant references to keep things safe
																							//while avoiding copying

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
};

#endif // !OBJECT_OBJECT_HANDLER_H_
