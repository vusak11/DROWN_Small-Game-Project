#include "object_class.h"

ObjectClass::ObjectClass() {
}

ObjectClass::~ObjectClass() {
}

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

glm::vec4 ObjectClass::GetPosition() const {
	return position_;
}

glm::vec4 ObjectClass::GetVelocity() const
{
	return velocity_;
}

glm::vec4 ObjectClass::GetRotation() const
{
	return rotation_;
}

glm::vec4 ObjectClass::GetScale() const
{
	return scale_;
}
