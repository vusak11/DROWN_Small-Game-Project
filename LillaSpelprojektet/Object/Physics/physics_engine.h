#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>
#include <cstdlib>	//Has abs()
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/vector_angle.hpp>

#include "../object_class.h"

class PhysicsEngine {
private:
	float gravitational_acceleration_;
	float object_max_velocity_;
	float object_min_velocity_;
	float object_ground_loss_ratio_;
	float object_air_loss_ratio_;

	std::vector<std::vector<float>>* map_height_list_;

	void UpdateVelocity(float& in_deltatime, ObjectClass*& in_object_ptr);	//Changes an object's velocity by applying acceleration
	void UpdatePosition(float& in_deltatime, ObjectClass*& in_object_ptr);	//Changes an object's position by applying velocity
	glm::vec3 CheckCollision(ObjectClass*& in_object_ptr, glm::vec3 new_pos);

public:
	PhysicsEngine(std::vector<std::vector<float>>* in_map_height_list);
	~PhysicsEngine();

	void ApplyPhysics(float& in_deltatime, std::vector<ObjectClass*>& in_object_ptr_vector);
};

#endif // !PHYSICS_ENGINE_H
