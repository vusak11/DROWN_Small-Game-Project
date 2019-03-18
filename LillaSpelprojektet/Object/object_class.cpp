#include "object_class.h"

#include <iostream>			//TEMP

//Private--------------------------------------------------

void ObjectClass::CalculateModelMatrix() {
	//Add all matrices together into a model matrix.
	//The order is important here.
	//First we scale, then we rotate and finally we translate
	this->model_matrix_ = this->translation_matrix_ * this->rotation_matrix_ * this->scaling_matrix_;
	this->UpdateHitbox(position_);
}

//Protected------------------------------------------------

void ObjectClass::SetObjectID(ObjectID object_id) {
	id_ = object_id;
}

//Public---------------------------------------------------

ObjectClass::ObjectClass(glm::vec3 start_pos, ObjectID id) {

	this->id_ = id;
	this->airborne_ = false;

	this->position_ = start_pos;	// start_pos;

	this->turn_rate_radians_ = glm::radians(GlobalSettings::Access()->ValueOf("OBJECT_TURN_RATE"));

	this->velocity_vec_ = { 0.0f,0.0f,0.0f };		// Not ilizializing this makes weiered start values.
	this->scale_ = glm::vec3(1.0f, 1.0f, 1.0f);

	int rotation_around_x_ = 0;
	int rotation_around_y_ = 0;
	int rotation_around_z_ = 0;

	this->scaling_matrix_ = glm::mat4(1.0f);
	this->rotation_matrix_ = glm::mat4(1.0f);
	this->translation_matrix_ = glm::mat4(1.0f);

	//hitbox_ = HitBox(position_, scale_.x, scale_.y);
	this->UpdateHitbox(position_);

	//TBA: Use the ID to determine the specs of a Object (Character/Drop/etc)

	this->model_matrix_up_to_date_ = false;

	this->velocity_vec_ = glm::vec3(0.0f);
	//this->acceleration_vec_ = glm::vec3(0.0f);
}

ObjectClass::~ObjectClass() {
}
 
bool ObjectClass::operator==(const ObjectClass& in_object) {
	//Checks if this object is compared to itself. If it is return true, otherwise false
	if (this == &in_object) { return true; }
	return false;
}

void ObjectClass::SetPosition(float in_x, float in_y, float in_z) {
	position_.x = in_x;
	position_.y = in_y;
	position_.z = in_z;

	//Translate an identity matrix to position_ and set it as the model's translation matrix
	this->translation_matrix_ = glm::translate(glm::mat4(1.0f), position_);

	//Model matrix is now out of date
	this->model_matrix_up_to_date_ = false;

	//Apply new position on the hitbox
	this->UpdateHitbox(position_);
}

void ObjectClass::SetScale(float in_s) {
	scale_ = glm::vec3(in_s, in_s, in_s);

	//Scale an identity matrix by scale_
	this->scaling_matrix_ = glm::scale(glm::mat4(1.0f), this->scale_);

	//Model matrix is now out of date
	this->model_matrix_up_to_date_ = false;

	//Apply new scale on the hitbox
	this->UpdateHitbox(position_);
}

void ObjectClass::SetScale(float in_x, float in_y, float in_z) {
	scale_ = glm::vec3(in_x, in_y, in_z);

	//Scale an identity matrix by scale_
	this->scaling_matrix_ = glm::scale(glm::mat4(1.0f), this->scale_);

	//Model matrix is now out of date
	this->model_matrix_up_to_date_ = false;
}

void ObjectClass::SetRotation(float in_x, float in_y, float in_z) {
	this->rotation_around_x_ = in_x;
	this->rotation_around_y_ = in_y;
	this->rotation_around_z_ = in_z;

	//Create three matrices for rotating around x, y and z
	glm::mat4 rotation_matrix_x = glm::rotate((float)this->rotation_around_x_, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotation_matrix_y = glm::rotate((float)this->rotation_around_y_, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotation_matrix_z = glm::rotate((float)this->rotation_around_z_, glm::vec3(0.0f, 0.0f, 1.0f));

	//Add the rotations together
	this->rotation_matrix_ = rotation_matrix_x * rotation_matrix_y * rotation_matrix_z;

	//World matrix is now out of date
	this->model_matrix_up_to_date_ = false;
}

void ObjectClass::SetUsingPhysics(bool use_physics) {
	use_physics_ = use_physics;
}

void ObjectClass::SetVelocity(float in_velocity) {
	//Normalize the current vector and then scale it in accordance with the new velocity
	this->velocity_vec_ = glm::normalize(this->velocity_vec_) * in_velocity;
}

void ObjectClass::SetVelocityVec(glm::vec3 in_velocity_vec) {
	//Set the velocity vector to be the new velocity
	this->velocity_vec_ = in_velocity_vec;
}

/*
void ObjectClass::SetAccelerationVec(glm::vec3 in_acceleration_vec) {
	//Set the acceleration vector to be the new velocity
	this->acceleration_vec_ = in_acceleration_vec;
}
*/

ObjectID ObjectClass::GetObjectID() const {
	return this->id_;
}

glm::vec3 ObjectClass::GetPosition() const {
	return this->position_;
}

glm::vec3 ObjectClass::GetScale() const
{
	return this->scale_;
}

float ObjectClass::GetVelocity() const {
	return glm::length(this->velocity_vec_);
}

glm::vec3 ObjectClass::GetVelocityVec() const {
	return this->velocity_vec_;
}

bool ObjectClass::GetUsePhysics() const {
	return use_physics_;
}

glm::mat4 ObjectClass::RetrieveModelMatrix() {
	//If the model matrix is not up to date call the function calculating it
	if (!this->model_matrix_up_to_date_) {
		this->CalculateModelMatrix();
	}
	
	return this->model_matrix_;
}

bool ObjectClass::IsAirborne() {
	return this->airborne_;
}

void ObjectClass::SetAirborne(bool in_bool) {
	this->airborne_ = in_bool;
}