#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>
#include <cstdlib>	//Has abs()

#include "../object_class.h"

class PhysicsEngine {
private:
	float gravitational_acceleration_;
	float object_max_velocity_;
	float object_min_velocity_;
	float object_decceleration_;

	std::vector<std::vector<float>>* map_height_list_;

	void UpdateVelocity(float& in_deltatime, ObjectClass*& in_object_ptr);	//Changes an object's velocity by applying acceleration
	void UpdatePosition(float& in_deltatime, ObjectClass*& in_object_ptr);	//Changes an object's position by applying velocity

public:
	PhysicsEngine(
		float in_gravitational_acceleration,
		float in_object_max_velocity,
		float in_object_min_velocity,
		float in_object_decceleration,
		std::vector<std::vector<float>>* in_map_height_list
	);
	~PhysicsEngine();

	void ApplyPhysics(float& in_deltatime, std::vector<ObjectClass*>& in_object_ptr_vector);
};

#endif // !PHYSICS_ENGINE_H
