#ifndef OBJECT_OBJECT_CLASS_H_
#define OBJECT_OBJECT_CLASS_H_

#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL		//Needed to use transform.hpp
#include <GLM/gtx/transform.hpp>

#include "../global_settings.h"
#include "hitbox.h"

// This class is used as a base for all the objects in the game. This goes for any
// objects or characters.
// Example:
//    class Character : ObjectClass

class ObjectClass : public Hitbox {
private:

	//Metadata/Math
	glm::vec3 position_;

	//Math			
	glm::vec3 scale_;			//Vector with	x scaling on x,
								//				y scaling on y
								//				and z scaling on z

	glm::mat4 translation_matrix_;
	glm::mat4 scaling_matrix_;
	glm::mat4 rotation_matrix_;

	bool model_matrix_up_to_date_;		//NTS:	Variable should be set to false anytime we change one of the other matrices
	glm::mat4 model_matrix_;

	//Physics
	bool use_physics_ = true;
	glm::vec3 velocity_vec_;

	//Functions----------------------------------------------
	void CalculateModelMatrix();	//Function calculates model matrix from scratch
protected:
	//Metadata
	ObjectID id_;
	bool airborne_;
	float turn_rate_radians_;

	//Math
	float rotation_around_y_;			//The rotation of the object in radians
										//around the y-axis

	void SetObjectID(ObjectID object_id);

public:
	ObjectClass(glm::vec3 start_pos = glm::vec3(0.0f, 0.0f, 0.0f), ObjectID id = OBJECT_ID_NULL);
	~ObjectClass();

	bool operator==(const ObjectClass& in_object);			//Equals operator for ObjectClass:es

	//Set Functions---------------------------------------
	void SetPosition(float in_x, float in_y, float in_z = 0.0f);
	void SetScale(float in_s);
	void SetScale(float in_x, float in_y, float in_z);
	void SetRotation(float in_y);
	
	//Physics
	void SetUsingPhysics(bool use_physics);
	void SetVelocity(float in_velocity);					//Set current velocity to in-parameter (does not change direction)
	void SetVelocityVec(glm::vec3 in_velocity_vec);			//Set current velocity and movement direction to match in-parameter

	//Get Functions----------------------------------------
	ObjectID GetObjectID() const;
	glm::vec3 GetPosition() const;				//Returns the object's x, y and z coordinates
	glm::vec3 GetScale() const;					//Returns the object's x, y and z scale variables
	float GetVelocity() const;					//Returns a float with the opject's velocity
	glm::vec3 GetVelocityVec() const;			//Returns a vec3 with the object's velocity vector
	bool GetUsePhysics() const;					// Returns if the object is using physics
	glm::mat4 RetrieveModelMatrix();			//NTS: Should check if model matrix is up to date before returning, and update it if it isn't
	//Other Functions--------------------------------------
	bool IsAirborne();
	virtual void SetAirborne(bool in_bool);


};


#endif // !OBJECT_OBJECT_CLASS_H_