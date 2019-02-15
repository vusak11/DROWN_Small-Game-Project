#include "physics_engine.h"
#include <iostream>

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

	///												

	// Do NOT change any of the map values
	//map_height_list_[4][3]
	int radius_constant = 10;
	int map_size = 100;	// OBS this variable needs to be collected from the map

	

	// Get intervals of points to check in the map data from the players position + a radius
	// This could probably be replaced by something less computation heavy
	int player_tile_pos_x = object_pos.x / MAP_SCALE;
	if ((int)object_pos.x % MAP_SCALE > MAP_SCALE)
	{
		player_tile_pos_x++;
	}
	int x_min = __max(player_tile_pos_x - radius_constant, 0);
	int x_max = __min(x_min + 2 * radius_constant, MAP_SCALE * map_size - MAP_SCALE);

	int player_tile_pos_y = object_pos.y / MAP_SCALE;
	if ((int)object_pos.y % MAP_SCALE > MAP_SCALE)
	{
		player_tile_pos_y++;
	}
	int y_min = __max(player_tile_pos_y - radius_constant, 0);
	int y_max = __min(y_min + 2 * radius_constant, MAP_SCALE * map_size - MAP_SCALE);

	bool collision_0 = false;
	bool collision_1 = false;
	bool collision_2 = false;
	bool collision_3 = false;

	// precalculate the hitbox position on the grid;

	HitBox tmp_hitbox = HitBox(glm::vec3(player_tile_pos_x, player_tile_pos_y, 0.0f), in_object_ptr->GetScale());

	//---------Point_0---------//
	int pos_0_x = tmp_hitbox.hb_pos0_.x / MAP_SCALE;
	if ((int)tmp_hitbox.hb_pos0_.x % MAP_SCALE > MAP_SCALE)
		pos_0_x++;

	int pos_0_y = tmp_hitbox.hb_pos0_.y / MAP_SCALE;
	if ((int)tmp_hitbox.hb_pos0_.y % MAP_SCALE > MAP_SCALE)
		pos_0_y++;

	//---------Point_1---------//

	int pos_1_x = tmp_hitbox.hb_pos1_.x / MAP_SCALE;
	if ((int)tmp_hitbox.hb_pos1_.x % MAP_SCALE > MAP_SCALE)
		pos_1_x++;

	int pos_1_y = tmp_hitbox.hb_pos1_.y / MAP_SCALE;
	if ((int)tmp_hitbox.hb_pos1_.y % MAP_SCALE > MAP_SCALE)
		pos_1_y++;

	//---------Point_2---------//

	int pos_2_x = tmp_hitbox.hb_pos2_.x / MAP_SCALE;
	if ((int)tmp_hitbox.hb_pos2_.x % MAP_SCALE > MAP_SCALE)
		pos_2_x++;

	int pos_2_y = tmp_hitbox.hb_pos2_.y / MAP_SCALE;
	if ((int)tmp_hitbox.hb_pos2_.y % MAP_SCALE > MAP_SCALE)
		pos_2_y++;

	//---------Point_3---------//

	int pos_3_x = tmp_hitbox.hb_pos3_.x / MAP_SCALE;
	if ((int)tmp_hitbox.hb_pos3_.x % MAP_SCALE > MAP_SCALE)
		pos_3_x++;

	int pos_3_y = tmp_hitbox.hb_pos3_.y / MAP_SCALE;
	if ((int)tmp_hitbox.hb_pos3_.y % MAP_SCALE > MAP_SCALE)
		pos_3_y++;


	//HitBox tmp_hitbox = HitBox(glm::vec3(player_tile_pos_x, player_tile_pos_y, 0.0f), in_object_ptr->GetScale());


	// Maybe loop this 3 times maximum in a foor loop?

	// Get through the interval and check each point which is in not in the background
	for (int i = x_min; i < x_max; i++)
	{
		for (int j = y_min; j < y_max; j++)
		{
			float height_index = (*map_height_list_)[map_size - i][j];
			if (height_index > 0.9f)
			{
				// if point 0 in hitbox is on a tile with a vertex from the map, collision_0 = true;
				//---------Point_0---------//
				if (pos_0_x == i && pos_0_y == j)
				{
					collision_0 = true;
				}
				//---------Point_1---------//
				if (pos_1_x == i && pos_1_y == j)
				{
					collision_1 = true;
				}
				//---------Point_2---------//
				if (pos_2_x == i && pos_2_y == j)
				{
					collision_2 = true;
				}
				//---------Point_3---------//
				if (pos_3_x == i && pos_3_y == j)
				{
					collision_3 = true;
				}
			}
		}
	}

	// Finally do things in a switch case if colliding.
	
	if (collision_0 && collision_1)	// Bot collision
	{
		object_pos.y++;
		std::cout << "Bot Collision" << std::endl;
	}
	else if (collision_0 && collision_3)	// Left collision
	{
		object_pos.x++;
		std::cout << "Left Collision" << std::endl;
	}
	else if (collision_1 && collision_2)	// Right collision
	{
		object_pos.x--;
		std::cout << "Right Collision" << std::endl;
	}
	else if (collision_3 && collision_2)	// Top collision
	{
		object_pos.y--;
		std::cout << "Top Collision" << std::endl;
	}
	else if (collision_0)
	{

		std::cout << "0 Collision" << std::endl;
	}
	else if (collision_1)
	{

		std::cout << "1 Collision" << std::endl;
	}
	else if (collision_2)
	{

		std::cout << "2 Collision" << std::endl;
	}
	else if (collision_3)
	{

		std::cout << "3 Collision" << std::endl;
	}

	// Reset the flags
	collision_0 = false;
	collision_1 = false;
	collision_2 = false;
	collision_3 = false;

	///												



	//TEMP: DON'T LET AN OBJECT BELOW THE "GROUND" PLANE
	//if (object_pos.y < 0.0f) { object_pos.y = 0.0f; }

	//Set the object's new position
	in_object_ptr->SetPosition(object_pos.x, object_pos.y);

}

//Public---------------------------------------------------
PhysicsEngine::PhysicsEngine(
	float in_gravitational_acceleration,
	float in_object_max_velocity,
	float in_object_min_velocity,
	float in_object_decceleration,
	std::vector<std::vector<float>>* in_map_height_list
) {
	this->gravitational_acceleration_ = in_gravitational_acceleration;
	this->object_max_velocity_ = in_object_max_velocity;
	this->object_min_velocity_ = in_object_min_velocity;
	this->object_decceleration_ = in_object_decceleration;
	this->map_height_list_ = in_map_height_list;
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