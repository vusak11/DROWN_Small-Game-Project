#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>

#include "../object_class.h"

class PhysicsEngine {
private:
	float gravitational_acceleration_;
	float object_max_velocity_;

	void UpdateVelocity(ObjectClass*& in_object_ptr);	//Changes an object's velocity by applying acceleration
	void UpdatePosition(ObjectClass*& in_object_ptr);	//Changes an object's position by applying velocity

public:
	PhysicsEngine(float in_gravitational_acceleration, float in_object_max_velocity);
	~PhysicsEngine();

	void ApplyPhysics(std::vector<ObjectClass*>& in_object_ptr_vector);
};

#endif // !PHYSICS_ENGINE_H
