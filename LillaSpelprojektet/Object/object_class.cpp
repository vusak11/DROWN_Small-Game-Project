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

void ObjectClass::SetRotation(int in_x, int in_y, int in_z) {
	this->rotation_around_x_ = in_x % 360;
	this->rotation_around_y_ = in_y % 360;
	this->rotation_around_z_ = in_z % 360;

	//Create three matrices for rotating around x, y and z
	glm::mat4 rotation_matrix_x = glm::rotate((float)this->rotation_around_x_, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotation_matrix_y = glm::rotate((float)this->rotation_around_y_, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotation_matrix_z = glm::rotate((float)this->rotation_around_z_, glm::vec3(0.0f, 0.0f, 1.0f));

	//Add the rotations together
	this->rotation_matrix_ = rotation_matrix_x * rotation_matrix_y * rotation_matrix_z;

	//World matrix is now out of date
	this->world_matrix_up_to_date_ = false;
}

void ObjectClass::SetVelocity(float in_velocity) {
	//If the velocity given is greater than our allowed maximum
	//clamp it down to it
	if (in_velocity > OBJECT_MAX_VELOCITY) { in_velocity = OBJECT_MAX_VELOCITY; }
	else if (in_velocity < -OBJECT_MAX_VELOCITY) { in_velocity = -OBJECT_MAX_VELOCITY; }

	//Normalize the current vector and then scale it in accordance with the new velocity
	this->velocity_vec_ = glm::normalize(this->velocity_vec_) * in_velocity;
}

void ObjectClass::SetVelocityVec(glm::vec3 in_velocity_vec) {
	//Set the velocity vector to be the new velocity
	this->velocity_vec_ = in_velocity_vec;

	//Then call the SetVelocity function to clamp the value below OBJECT_MAX_VELOCITY
	this->SetVelocity(glm::length(this->velocity_vec_));

}

glm::vec3 ObjectClass::GetPosition() const {
	return this->position_;
}

float ObjectClass::GetVelocity() const {
	return glm::length(this->velocity_vec_);
}

glm::vec3 ObjectClass::GetVelocityVec() const {
	return this->velocity_vec_;
}

glm::mat4 ObjectClass::GetWorldMatrix() {
	//If the world matrix is not up to date call the function calculating it
	if (!this->world_matrix_up_to_date_) { this->CalculateWorldMatrix(); }
	
	return this->world_matrix_;
}

void ObjectClass::UpdatePosition(float in_deltatime) {
	//Updates object position in accordance with how far its velocity would have taken it
	this->position_ = this->position_ + (in_deltatime * this->velocity_vec_);

	//NTS: This function has no stops. It does not stop by walls. Keep in mind that even with
	//the function moving back if we end up in a wall enough velocity would just carry us through it
}


void ObjectClass::AlterVelocityVec(glm::vec3 in_vec) {
	//Alter the current velocity vector with the new given one
	this->velocity_vec_ = this->velocity_vec_ + in_vec;

	//Then call the SetVelocity function to clam the value below OBJECT_MAX_VELOCITY
	this->SetVelocity(glm::length(this->velocity_vec_));
}

void ObjectClass::TurnLeft() {
	//Turn the model leftwards (positive direction)
	int new_rotation = (this->rotation_around_y_ + OBJECT_TURN_RATE) % 360;

	//If the new orientation lies somewhere in [90, 180] we have turned too far
	//and we snap back to 90
	if ((new_rotation > 90) && (new_rotation < 180)) { new_rotation = 90; }

	this->SetRotation(this->rotation_around_x_, new_rotation, this->rotation_around_z_);
}

void ObjectClass::TurnRight() {
	//Turn the model rightwards (negative direction)
	int new_rotation = (this->rotation_around_y_ - OBJECT_TURN_RATE) % 360;

	//If the new orientation lies somewhere in [180, 270] we have turned too far
	//and we snap back to 270
	if ((new_rotation > 180) && (new_rotation < 270)) { new_rotation = 270; }

	this->SetRotation(this->rotation_around_x_, new_rotation, this->rotation_around_z_);
}