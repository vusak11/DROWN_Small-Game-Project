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

	//Apply the object's acceleration to its velocity
	//v = v0 + a*t
	velocity_vec = velocity_vec + in_object_ptr->GetAccelerationVec() * in_deltatime;

	//Apply gravity to the y velocity
	//v = v0 + g*t
	velocity_vec.y = velocity_vec.y + this->gravitational_acceleration_ * in_deltatime;

	//Apply the x-axis decceleration (Different for ground/air)
	//v = v0 + (-v0*lr)*t;
	float loss_ratio = (in_object_ptr->object_metadata_.airborne) ?
		this->object_air_loss_ratio_ :
		this->object_ground_loss_ratio_;
	
	//v = v(1-d);
	//v = v + (-v)d*t
	float decceleration = -velocity_vec.x*loss_ratio;

	velocity_vec.x = velocity_vec.x + decceleration * in_deltatime;

	//If the new velocity is too low set it to 0
	if (std::abs(velocity_vec.x) < this->object_min_velocity_) { velocity_vec.x = 0.0f; }

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
	if (object_pos.y < 0.0f) {
		object_pos.y = 0.0f;
		glm::vec3 grounded_velocity = in_object_ptr->GetVelocityVec();
		grounded_velocity.y = 0.0f;
		in_object_ptr->SetVelocityVec(grounded_velocity);
		in_object_ptr->object_metadata_.airborne = false;
	}

	//Set the object's new position
	in_object_ptr->SetPosition(object_pos.x, object_pos.y);

}

//Public---------------------------------------------------
PhysicsEngine::PhysicsEngine() {
	this->gravitational_acceleration_	= GRAVITATIONAL_ACCELERATION;
	this->object_max_velocity_			= OBJECT_MAX_VELOCITY;
	this->object_min_velocity_			= OBJECT_MIN_VELOCITY;
	this->object_ground_loss_ratio_		= OBJECT_GROUND_LOSS_RATIO;
	this->object_air_loss_ratio_		= OBJECT_AIR_LOSS_RATIO;
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