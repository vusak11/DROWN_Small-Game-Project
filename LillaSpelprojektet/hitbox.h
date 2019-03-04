#ifndef HITBOX_H_
#define HITBOX_H_

#include <GLM/glm.hpp>


// This class is used to get hitbox points for an object.
// Example:
//    object_.hitbox_.GetPoint0;

//    3       2
//	  ________ 
//	  |		  | 
//	  |  Box  |
//	  |		  | 
//	  ---------
//    0       1

struct BoxPoints {
	glm::vec2 bottomLeft;
	glm::vec2 bottomRight;
	glm::vec2 topLeft;
	glm::vec2 topRight;
};

class Hitbox {
private:
	glm::vec3 position_;
	float x_offset_;
	float y_offset_;

public:
	Hitbox();
	Hitbox(glm::vec3 position, float x_offset, float y_offset);
	~Hitbox();

	// Call this each time the object is translated in the world to translate the hitbox
	void UpdateHitbox(glm::vec3 position, float x_offset, float y_offset);

	glm::vec2 GetPoint0() const;
	glm::vec2 GetPoint1() const;
	glm::vec2 GetPoint2() const;
	glm::vec2 GetPoint3() const;

	// Returns a list of the 4 hitbox points
	BoxPoints GetPoints() const;

	// Compare this Hitbox with sent one and return true if they intersect
	virtual bool CheckCollision(const BoxPoints& other_box);
};




#endif
