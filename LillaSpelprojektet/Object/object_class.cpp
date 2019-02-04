#include "object_class.h"

//Private--------------------------------------------------

void ObjectClass::CalculateWorldMatrix() {
	//Add all matrices together into a world matrix.
	//The order is important here.
	//First we scale, then we rotate and finally we translate
	this->world_matrix_ = this->translation_matrix_ * this->rotation_matrix_ * this->scaling_matrix_;
}


//Public---------------------------------------------------

ObjectClass::ObjectClass() {
	this->world_matrix_up_to_date_ = false;
}

ObjectClass::~ObjectClass() {
}
 

void ObjectClass::SetPosition(float in_x, float in_y, float in_z = 0.0f) {
	position_.x = in_x;
	position_.y = in_y;
	position_.z = in_z;

	//Translate an identity matrix to position_ and set it as the model's translation matrix
	this->translation_matrix_ = glm::translate(glm::mat4(), position_);

	//World matrix is now out of date
	this->world_matrix_up_to_date_ = false;
}

void ObjectClass::SetScale(float in_s) {
	scale_ = glm::vec3(in_s, in_s, in_s);

	//Scale an identity matrix by scale_
	this->scaling_matrix_ = glm::scale(glm::mat4(), this->scale_);

	//World matrix is now out of date
	this->world_matrix_up_to_date_ = false;
}

void ObjectClass::SetScale(float in_x, float in_y, float in_z) {
	scale_ = glm::vec3(in_x, in_y, in_z);

	//Scale an identity matrix by scale_
	this->scaling_matrix_ = glm::scale(glm::mat4(), this->scale_);

	//World matrix is now out of date
	this->world_matrix_up_to_date_ = false;
}

void ObjectClass::SetRotation(float in_x, float in_y, float in_z) {
	this->rotation_around_x_ = in_x;
	this->rotation_around_y_ = in_y;
	this->rotation_around_z_ = in_z;

	//Create three matrices for rotating around x, y and z
	glm::mat4 rotation_matrix_x = glm::rotate(this->rotation_around_x_, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotation_matrix_y = glm::rotate(this->rotation_around_y_, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotation_matrix_z = glm::rotate(this->rotation_around_z_, glm::vec3(0.0f, 0.0f, 1.0f));

	//Add the rotations together
	this->rotation_matrix_ = rotation_matrix_x * rotation_matrix_y * rotation_around_z_;

	//World matrix is now out of date
	this->world_matrix_up_to_date_ = false;
}



/*
void ObjectClass::SetVelocity(glm::vec4 const v) {
	velocity_ = v;
}


*/

glm::vec3 ObjectClass::GetPosition() const {
	return this->position_;
}

glm::vec4 ObjectClass::GetVelocityVector() const {
	//Calculates a vec4 describing the directional velocity and returns it
	return glm::vec4(this->velocity_ * glm::normalize(this->move_direction_), 0.0);
}

glm::mat4 ObjectClass::GetWorldMatrix() {
	//If the world matrix is not up to date call the function calculating it
	if (!this->world_matrix_up_to_date_) { this->CalculateWorldMatrix(); }
	
	return this->world_matrix_;
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