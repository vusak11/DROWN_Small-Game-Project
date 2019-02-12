#include "physics_engine.h"

//Private--------------------------------------------------


void PhysicsEngine::UpdateVelocity(ObjectClass*& in_object_ptr) {

	//WIP
	//If the new velocity is greater than our allowed maximum
	//clamp it down to it
	//if (in_velocity > OBJECT_MAX_VELOCITY) {
	//	in_velocity = OBJECT_MAX_VELOCITY;
	//}
	//else if (in_velocity < -OBJECT_MAX_VELOCITY) {
	//	in_velocity = -OBJECT_MAX_VELOCITY;
	//}
}

void PhysicsEngine::UpdatePosition(ObjectClass*& in_object_ptr) {

}

//Public---------------------------------------------------
PhysicsEngine::PhysicsEngine(float in_gravitational_acceleration, float in_object_max_velocity) {
	this->gravitational_acceleration_ = in_gravitational_acceleration;
	this->object_max_velocity_ = in_object_max_velocity;
}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::ApplyPhysics(std::vector<ObjectClass*>& in_object_ptr_vector) {

}