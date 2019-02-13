#include "physics_engine.h"

//Private--------------------------------------------------


void PhysicsEngine::UpdateVelocity(float& in_deltatime, ObjectClass*& in_object_ptr) {

	//WIP
	//If the new velocity is greater than our allowed maximum
	//clamp it down to it
	//if (in_velocity > OBJECT_MAX_VELOCITY) {
	//	in_velocity = OBJECT_MAX_VELOCITY;
	//}
	//else if (in_velocity < -OBJECT_MAX_VELOCITY) {
	//	in_velocity = -OBJECT_MAX_VELOCITY;
	//}

	//Get the object's velocity
	glm::vec3 velocity_vec = in_object_ptr->GetVelocityVec();

	//Apply gravity to the y velocity
	//v = v0 + g*t
	velocity_vec.y = velocity_vec.y + this->gravitational_acceleration_ * in_deltatime;

	//Apply the x-axis decceleration
	//If the new velocity is too low set it to 0
	//v = v*(1-d)
	velocity_vec.x = velocity_vec.x * (1 - this->object_decceleration_);
	if (velocity_vec.x < this->object_min_velocity_) { velocity_vec.x = 0.0f; }

	//Check if the new velocity exceeds maximum and if so clamp it to max
	if (glm::length(velocity_vec) > this->object_max_velocity_) {
		velocity_vec = glm::normalize(velocity_vec) * this->object_max_velocity_;
	}

	//Set the new velocity of the object
	in_object_ptr->SetVelocityVec(velocity_vec);

}

void PhysicsEngine::UpdatePosition(float& in_deltatime, ObjectClass*& in_object_ptr) {

	//Get the object's position
	glm::vec3 object_pos = in_object_ptr->GetPosition();

	//Displace the object using its velocity during deltatime
	object_pos += in_object_ptr->GetVelocityVec()*in_deltatime;

	//TBA(?): COLLISION DETECTION VS MAP

	//TEMP: DON'T LET AN OBJECT BELOW THE "GROUND" PLANE
	if (object_pos.y < 0.0f) { object_pos.y = 0.0f; }

	//Set the object's new position
	in_object_ptr->SetPosition(object_pos.x, object_pos.y);

}

//Public---------------------------------------------------
PhysicsEngine::PhysicsEngine(
	float in_gravitational_acceleration,
	float in_object_max_velocity,
	float in_object_min_velocity,
	float in_object_decceleration
) {
	this->gravitational_acceleration_ = in_gravitational_acceleration;
	this->object_max_velocity_ = in_object_max_velocity;
	this->object_min_velocity_ = in_object_min_velocity;
	this->object_decceleration_ = in_object_decceleration;
}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::ApplyPhysics(float& in_deltatime, std::vector<ObjectClass*>& in_object_ptr_vector) {
	
	//Loop through all objects in vector
	for (unsigned int i = 0; i < in_object_ptr_vector.size(); i++) {
		this->UpdateVelocity(in_deltatime, in_object_ptr_vector.at(i));
		this->UpdatePosition(in_deltatime, in_object_ptr_vector.at(i));
		//TBA(?): COLLISION DETECTION VS MAP
	}


}