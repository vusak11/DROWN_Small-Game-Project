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
	float top_speed_;
	float jump_speed_;

	Weapon weapon_;
	Ability ability_;
	

public:
	PlayerCharacter(glm::vec3 start_pos);
	~PlayerCharacter();

	void MoveLeft(const float& in_deltatime);
	void MoveRight(const float& in_deltatime);
	void Jump();

};


#endif // !PLAYER_CHARACTER_H
