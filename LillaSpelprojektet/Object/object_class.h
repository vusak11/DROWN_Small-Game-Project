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
public:
	ObjectClass();
	~ObjectClass();

	void SetPosition(glm::vec4 const pos);
	void SetVelocity(glm::vec4 const v);
	void SetRotation(glm::vec4 const r);
	void SetScale(glm::vec4 const s);

	glm::vec4 GetPosition() const;
	glm::vec4 GetVelocity() const;
	glm::vec4 GetRotation() const;
	glm::vec4 GetScale() const;

private:
	glm::mat4 world_matrix_;
	glm::vec4 position_;
	glm::vec4 velocity_;

	glm::vec4 rotation_;
	glm::vec4 scale_;

	//Model* model_; // The model class will contain a TextureClass 
	//HitBot hitbox_; 
};


#endif // !OBJECT_OBJECT_CLASS_H_