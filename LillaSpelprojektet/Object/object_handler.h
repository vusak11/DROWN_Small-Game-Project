#ifndef OBJECT_OBJECT_HANDLER_H_
#define OBJECT_OBJECT_HANDLER_H_

#include <vector>
#include <functional>		//Allows function pointers and lambdas
							//Ref. for how it works: https://en.cppreference.com/w/cpp/utility/functional/function

#include "object_class.h"

class ObjectHandler {
private:

	//Variables for objects in the game
	ObjectClass player_object_;				//Player
	std::vector<ObjectClass> npc_vector_;	//All enemies
	std::vector<ObjectClass> drop_vector_;	//Things dropped on the ground (e.g. power-ups, health)

	//Variables for controlling the player's behavious
	//Vector of player functions

public:
	ObjectHandler();
	~ObjectHandler();

	//Functions for controlling the player
	//Call these functions from eventhandler
	void PlayerMove(/*Input*/);
	void PlayerAttack();
	void PlayerAbility();
	void PlayerPickUp();

	//Called once per loop to update object positions
	//	- Read player inputs and determine effects
	//	- Call AI functions and decide behaviour
	//	- Execute NPC actions and determine effects
	//	- Call physics to determine new positions (<- Should be called at every step when a creature moves instead?)
	void UpdateObjects();
};

#endif // !OBJECT_OBJECT_HANDLER_H_
