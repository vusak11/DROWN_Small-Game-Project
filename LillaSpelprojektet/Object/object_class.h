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


class ObjectClass {
private:
	//Variables-----------------------------------------------
	ObjectID id_;
	
	glm::vec3 position_;
	//glm::vec3 rotation_;		//Vector with	rotation around x-axis on x,
								//				rotation around y-axis on y
								//				and rotation around z-axis on z
								//Rotation represented in degrees

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

	glm::vec3 velocity_vec_;

	//Functions----------------------------------------------
	void CalculateModelMatrix();	//Function calculates model matrix from scratch

	//Model* model_; // The model class will contain a TextureClass 
	//HitBot hitbox_;

public:

	//WIP

	struct ObjectMetadata {				//An object's metadata tell us things
		bool airborne = false;					//about the object's current situation
	};
	ObjectMetadata object_metadata_;

	//

	ObjectClass(glm::vec3 start_pos = glm::vec3(0.0f, 0.0f, 0.0f), ObjectID id = OBJECT_ID_NULL);
	~ObjectClass();

	bool operator==(const ObjectClass& in_object);			//Equals operator for ObjectClass:es

	//Set Functions---------------------------------------
	void SetPosition(float in_x, float in_y, float in_z = 0.0f);
	void SetScale(float in_s);
	void SetScale(float in_x, float in_y, float in_z);
	void SetRotation(int in_x, int in_y, int in_z);
	void SetVelocity(float in_velocity);					//Set current velocity to in-parameter (does not change direction)
	void SetVelocityVec(glm::vec3 in_velocity_vec);			//Set curren velocity and movement direction to match in-parameter
	
	//Get Functions----------------------------------------
	ObjectID GetObjectID() const;
	glm::vec3 GetPosition() const;				//Returns the object's x, y and z coordinates
	float GetVelocity() const;					//Returns a float with the opject's velocity
	glm::vec3 GetVelocityVec() const;			//Returns a vec3 with the object's velocity vector
	glm::mat4 GetModelMatrix();					//NTS: Should check if model matrix is up to date before returning, and update it if it isn't

	//Other Functions--------------------------------------
	void UpdatePosition(float in_deltatime);		//Moves object along it's velocity vector in accorance with time elapsed
	void AlterVelocityVec(glm::vec3 in_vec);		//Add in parameter to current velocity and use result as new velocity
	void TurnLeft();
	void TurnRight();


};


#endif // !OBJECT_OBJECT_CLASS_H_