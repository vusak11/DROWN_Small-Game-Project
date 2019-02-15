#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "character.h"

//WIP: Placeholder for proper weapons and abilities
enum Weapon {
	SWORD
};

enum Ability {
	NONE,
	DOUBLE_JUMP
};
//

class PlayerCharacter : public Character {
private:
	//Player stats
	float move_speed_;		//player movement on x-axis
	float jump_speed_;		//speed that is set when player jumps

	Weapon weapon_;
	Ability ability_;
	

public:
	PlayerCharacter(glm::vec3 start_pos);
	~PlayerCharacter();

	void MoveLeft();
	void MoveRight();
	void Jump();

};


#endif // !PLAYER_CHARACTER_H
