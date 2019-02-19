#ifndef OBJECT_OBJECT_CLASS_H_
#define OBJECT_OBJECT_CLASS_H_

#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL		//Needed to use transform.hpp
#include <GLM/gtx/transform.hpp>

#include "../globals.h"

// This class is used as a base for all the objects in the game. This goes for any
// objects or characters.
// Example:
//    class Character : ObjectClass

struct HitBox // Struct used for checking collision
	{
		//    3       2
		//	  ________ 
		//	  |		  | 
		//	  |  Box  |
		//	  |		  | 
		//	  ---------
		//    0       1
		glm::vec2 hb_pos0_;
		glm::vec2 hb_pos1_;
		glm::vec2 hb_pos2_;
		glm::vec2 hb_pos3_;

		HitBox() {}
		HitBox(glm::vec3 p, glm::vec3 s) {
			// Initialize the hitbox to player position.
			hb_pos0_ = glm::vec2(p.x + (-1 * s.x), p.y + (-1 * s.y));
			hb_pos1_ = glm::vec2(p.x + ( 1 * s.x), p.y + (-1 * s.y));
			hb_pos2_ = glm::vec2(p.x + ( 1 * s.x), p.y + ( 1 * s.y));
			hb_pos3_ = glm::vec2(p.x + (-1 * s.x), p.y + ( 1 * s.y));
		}
		~HitBox(){}
		void Update(glm::vec3 p, glm::vec3 s) {
			hb_pos0_ = glm::vec2(p.x + (-1 * s.x), p.y + (-1 * s.y));
			hb_pos1_ = glm::vec2(p.x + (1 * s.x), p.y + (-1 * s.y));
			hb_pos2_ = glm::vec2(p.x + (1 * s.x), p.y + (1 * s.y));
			hb_pos3_ = glm::vec2(p.x + (-1 * s.x), p.y + (1 * s.y));
		}
	};

class ObjectClass {
private:

	

	//Variables-----------------------------------------------
	ObjectID id_;
	HitBox hit_box_;


	//Metadata/Math
	glm::vec3 position_;

	//Math
	int rotation_around_x_;			//The rotation of the object in degrees 
	int rotation_around_y_;			//around the x, y and z-axises
	int rotation_around_z_;			//NTS:	0 rotation around y is default (model look towards screen[positive z])
									//		-90 (or 270) rotation around y is looking towards the right[positive x]
									//		90 rotation around y is looking towards the left[negative x]

	glm::vec3 scale_;			//Vector with	x scaling on x,
								//				y scaling on y
								//				and z scaling on z

	glm::mat4 translation_matrix_;
	glm::mat4 scaling_matrix_;
	glm::mat4 rotation_matrix_;

	bool model_matrix_up_to_date_;		//NTS:	Variable should be set to false anytime we change one of the other matrices
	glm::mat4 model_matrix_;

	//Physics
	glm::vec3 velocity_vec_;
	glm::vec3 acceleration_vec_;

	//Functions----------------------------------------------
	void CalculateModelMatrix();	//Function calculates model matrix from scratch

protected:
	//Metadata
	ObjectID id_;
	bool airborne_;
	//HitBot hitbox_;

public:
	ObjectClass(glm::vec3 start_pos = glm::vec3(0.0f, 0.0f, 0.0f), ObjectID id = OBJECT_ID_NULL);
	~ObjectClass();

	bool operator==(const ObjectClass& in_object);			//Equals operator for ObjectClass:es

	//Set Functions---------------------------------------
	void SetPosition(float in_x, float in_y, float in_z = 0.0f);
	void SetScale(float in_s);
	void SetScale(float in_x, float in_y, float in_z);
	void SetRotation(int in_x, int in_y, int in_z);
	
	//Physics
	void SetVelocity(float in_velocity);					//Set current velocity to in-parameter (does not change direction)
	void SetVelocityVec(glm::vec3 in_velocity_vec);			//Set current velocity and movement direction to match in-parameter
	void SetAccelerationVec(glm::vec3 in_acceleration_vec);	//Set current acceleration to in-parameter

	//Get Functions----------------------------------------
	ObjectID GetObjectID() const;
	HitBox GetHitBox();					//Returns the hitbox points of the object
	glm::vec3 GetPosition() const;				//Returns the object's x, y and z coordinates
	glm::vec3 GetScale() const;					//Returns the object's x, y and z scale variables
	float GetVelocity() const;					//Returns a float with the opject's velocity
	glm::vec3 GetVelocityVec() const;			//Returns a vec3 with the object's velocity vector
	glm::vec3 GetAccelerationVec() const;		//Returns a vec3 with the object's acceleration vector 
	glm::mat4 GetModelMatrix();					//NTS: Should check if model matrix is up to date before returning, and update it if it isn't

	//Other Functions--------------------------------------
	void TurnLeft(const float& in_deltatime);
	void TurnRight(const float& in_deltatime);
	bool IsAirborne();
	virtual void SetAirborne(bool in_bool);


};


#endif // !OBJECT_OBJECT_CLASS_H_