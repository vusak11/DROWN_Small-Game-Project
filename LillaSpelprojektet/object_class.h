#pragma once

#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>
//#include "model_class"

// Forward declarations
class Model;

// This class is used as a base for all the objects in the game. This goes for any
// objects and characters.
// Example:
//    class Character : ObjectClass


class ObjectClass
{
public:
	ObjectClass();
	~ObjectClass();

	virtual void Render() = 0; // Update this when more other things are implemented

	void SetPosition(glm::vec4 const pos);
	void SetVelocity(glm::vec4 const v);
	void SetRotation(glm::vec4 const r);
	void SetScale(glm::vec4 const s);

	void SetModel(); // Update ths when model reader system is working

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

	Model* model_; // The model class will contain a TextureClass 
	//HitBot hitbox_; 
};
