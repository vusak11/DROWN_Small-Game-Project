#ifndef OBJECT_OBJECT_CLASS_H_
#define OBJECT_OBJECT_CLASS_H_

#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
//#include "model_class"

// Forward declarations
class Model;

// This class is used as a base for all the objects in the game. This goes for any
// objects and characters.
// Example:
//    class Character : ObjectClass


class ObjectClass {
private:
	//Variables-----------------------------------------------
	glm::vec3 position_;
	glm::vec3 rotation_;		//Vector with	rotation around x-axis on x,
								//				rotation around y-axis on y
								//				and rotation around z-axis on z
	glm::vec3 scale_;			//Vector with	x scaling on x,
								//				y scaling on y
								//				and z scaling on z

	bool up_to_date_;				//NTS: A variable that should be set to false every time position, rotation or scale is updated
	glm::mat4 world_matrix_;

	float velocity_;
	glm::vec3 move_direction_;

	//Functions----------------------------------------------
	void CalculateWorldMatrix();	//NTS: This function should be called before returning the world matrix if up_to_date_ is false

	//Model* model_; // The model class will contain a TextureClass 
	//HitBot hitbox_;

public:
	ObjectClass();
	~ObjectClass();

	void SetPosition(glm::vec4 const pos);
	void SetVelocity(glm::vec4 const v);
	void SetRotation(glm::vec4 const r);
	void SetScale(glm::vec4 const s);

	glm::vec3 GetPosition() const;				//Returns the object's x, y and z coordinates
	glm::vec4 GetVelocityVector() const;		//Returns a vec4 with the object's velocity vector (direction of movement times velocity)

	glm::mat4 GetWorldMatrix();					//NTS: Function not 'const' declared as it should update the matrix if it isn't up to date

	//glm::vec4 GetRotation() const;
	//glm::vec4 GetScale() const;


};


#endif // !OBJECT_OBJECT_CLASS_H_