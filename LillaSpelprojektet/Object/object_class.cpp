#include "object_class.h"

//Private--------------------------------------------------




//Public---------------------------------------------------

ObjectClass::ObjectClass() {
}

ObjectClass::~ObjectClass() {
}
 
/*
void ObjectClass::SetPosition(glm::vec4 const pos) {
	position_.x = pos.x;
	position_.y = pos.y;
	position_.z = pos.z;
	position_.w = pos.w;
}

void ObjectClass::SetVelocity(glm::vec4 const v) {
	velocity_ = v;
}

void ObjectClass::SetRotation(glm::vec4 const r) {
	rotation_ = r;
}

void ObjectClass::SetScale(glm::vec4 const s) {
	scale_ = s;
}
*/

glm::vec3 ObjectClass::GetPosition() const {
	return this->position_;
}

glm::vec4 ObjectClass::GetVelocityVector() const {
	//Calculates a vec4 describing the directional velocity
	return glm::vec4(this->velocity_ * glm::normalize(this->move_direction_), 0.0);
}

/*
glm::vec4 ObjectClass::GetRotation() const
{
	return rotation_;
}

glm::vec4 ObjectClass::GetScale() const
{
	return scale_;
}
*/