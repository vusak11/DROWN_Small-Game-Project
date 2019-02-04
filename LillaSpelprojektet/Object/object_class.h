#ifndef OBJECT_OBJECT_CLASS_H_
#define OBJECT_OBJECT_CLASS_H_

#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>

// This class is used as a base for all the objects in the game. This goes for any
// objects or characters.
// Example:
//    class Character : ObjectClass


class ObjectClass {
private:
	//Variables-----------------------------------------------
	glm::vec3 position_;
	//glm::vec3 rotation_;		//Vector with	rotation around x-axis on x,
								//				rotation around y-axis on y
								//				and rotation around z-axis on z
								//Rotation represented in degrees

	float rotation_around_x_;			//The rotation of the object in degrees 
	float rotation_around_y_;			//around the x, y and z-axises
	float rotation_around_z_;			//

	glm::vec3 scale_;			//Vector with	x scaling on x,
								//				y scaling on y
								//				and z scaling on z



	glm::mat4 translation_matrix_;
	glm::mat4 scaling_matrix_;
	glm::mat4 rotation_matrix_;

	bool world_matrix_up_to_date_;		//NTS:	Variable should be set to false anytime we change one of the other matrices
	glm::mat4 world_matrix_;

	float velocity_;
	glm::vec3 move_direction_;

	//Functions----------------------------------------------
	void CalculateWorldMatrix();	//Function calculates world matrix from scratch

	//Model* model_; // The model class will contain a TextureClass 
	//HitBot hitbox_;

public:
	ObjectClass();
	~ObjectClass();

	void SetPosition(float in_x, float in_y, float in_z = 0.0f);

	void SetScale(float in_s);
	void SetScale(float in_x, float in_y, float in_z);

	void SetRotation(float in_x, float in_y, float in_z);
	
	
	void SetVelocity(glm::vec4 const v);
	
	

	glm::vec3 GetPosition() const;				//Returns the object's x, y and z coordinates
	glm::vec4 GetVelocityVector() const;		//Returns a vec4 with the object's velocity vector (direction of movement times velocity)

	glm::mat4 GetWorldMatrix();					//NTS: Should check if world matrix is up to date before returning, and update it if it isn't

	void UpdatePosition(float in_deltatime);	//Moves object along it's velocity vector in accorance with time elapsed

	//glm::vec4 GetRotation() const;
	//glm::vec4 GetScale() const;


};


#endif // !OBJECT_OBJECT_CLASS_H_