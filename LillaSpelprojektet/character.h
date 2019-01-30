#pragma once

#include <GLM/vec4.hpp>

// Forward declarations
class Model;

// This class is used as a base of each character in the game. 
// Example:
//    class Player : Character
//    class FireGuy : Character

class Character {
public:
	Character();
	~Character();

	virtual void Render(); // Update this when more other things are implemented

	void SetPosition(glm_vec4 const pos);
	void SetHealth(int const health);
	void SetPower(int const power);
	void SetModel(); // Update ths when model reader system is working

	glm_vec4 GetPosition();
	int GetHealth();
	int GetPower();
	Model* GetModel();
private:
	glm_vec4 mPosition_;
	int mHealth_;
	int mPower_ = 1;
	
	Model* mModel_;
};
