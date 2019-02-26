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




class HitBox
{
public:
	HitBox();
	HitBox(glm::vec3 position, float x_offset, float y_offset);
	~HitBox();

	// Compare this Hitbox with sent ont and return true if they intersect
	bool CheckCollision(HitBox other_box);

	// Call this each time the object is translated in the world to translate the hitbox
	void UpdateHitbox(glm::vec3 position, float x_offset, float y_offset);

	glm::vec2 GetPoint0() const;
	glm::vec2 GetPoint1() const;
	glm::vec2 GetPoint2() const;
	glm::vec2 GetPoint3() const;

	// Returns a list of the 4 hitbox points
	glm::vec2* GetPoints() const;
private:
	glm::vec3 position_;
	float x_offset_;
	float y_offset_;
};




#endif
