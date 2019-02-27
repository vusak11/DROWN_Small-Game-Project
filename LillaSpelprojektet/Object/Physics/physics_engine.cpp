#include "physics_engine.h"
#include <iostream>

//Private--------------------------------------------------


void PhysicsEngine::UpdateVelocity(float& in_deltatime, ObjectClass*& in_object_ptr) {

	//Get the object's velocity
	glm::vec3 velocity_vec = in_object_ptr->GetVelocityVec();

	//Apply the object's acceleration to its velocity
	//v = v0 + a*t
	//velocity_vec = velocity_vec + in_object_ptr->GetAccelerationVec() * in_deltatime;

	//Apply gravity to the y velocity
	//v = v0 + g*t
	velocity_vec.y = velocity_vec.y + this->gravitational_acceleration_ * in_deltatime;

	//Apply the x-axis decceleration (Different for ground/air)
	//v = v0 + (-v0*lr)*t;
	float loss_ratio = (in_object_ptr->IsAirborne()) ?
		this->object_air_loss_ratio_ :
		this->object_ground_loss_ratio_;
	
	//v = v(1-d);
	//v = v + (-v)d*t
	float decceleration = -velocity_vec.x*loss_ratio;

	velocity_vec.x = velocity_vec.x + decceleration *in_deltatime;

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

	int map_size = MAP_SIZE;	// OBS this variable needs to be collected from the map
	int radius_constant = 5;

	// | 1 | Get new position from acceleration and velocity.

	//Get the object's position
	glm::vec3 object_pos = in_object_ptr->GetPosition();
	//Displace the object using its velocity during deltatime
	object_pos += in_object_ptr->GetVelocityVec()*in_deltatime;

	/*
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
	x_min = __min(x_min, MAP_SCALE * map_size - MAP_SCALE);
	int x_max = __min(x_min + 2 * radius_constant, MAP_SCALE * map_size - MAP_SCALE);
	x_max = __max(x_max, 0);

	int player_tile_pos_y = object_pos.y / MAP_SCALE;
	if ((int)object_pos.y % MAP_SCALE > MAP_SCALE)
	{
		player_tile_pos_y++;
	}
	int y_min = __max(player_tile_pos_y - radius_constant, 0);
	y_min = __min(y_min, MAP_SCALE * map_size - MAP_SCALE);
	int y_max = __min(y_min + 2 * radius_constant, MAP_SCALE * map_size - MAP_SCALE);
	y_max = __max(y_max, 0);

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
	if (x_min < 0)
	{
		int x = 0;
	}

	// Maybe loop this 3 times maximum in a foor loop?

	// Get through the interval and check each point which is in not in the background
	for (int i = x_min; i < x_max; i++)
	{
		for (int j = y_min; j < y_max; j++)
		{
			float height_index = (*map_height_list_)[map_size - i][j];
			if (height_index > 15.0f)	// Height map is not normalized. currently height 20
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
	*/


	
	// Do collision check 3 times each frame
	for (int k = 0; k < 3; k++)
	{

		// Reset collosion flags
		bool collision_0 = false;
		bool collision_1 = false;
		bool collision_2 = false;
		bool collision_3 = false;

		// | 2 | Make values to 4 points surrounding the object like a box.
		float x_0 = object_pos.x - in_object_ptr->GetScale().x;
		float x_1 = __min((object_pos.x + in_object_ptr->GetScale().x), 495);
		float y_0 = object_pos.y - in_object_ptr->GetScale().y;
		float y_1 = __min((object_pos.y + in_object_ptr->GetScale().y), 495);

		// | 3 | Normalize each pos to later match with map location.
		x_0 = x_0 / (map_size - 1);
		x_1 = x_1 / (map_size - 1);
		y_0 = y_0 / (map_size - 1);
		y_1 = y_1 / (map_size - 1);


		// Get map index from normalized value
		int x_0_index = x_0 * (map_size - 1);
		float stair_adjustment_value_X_0 = x_0 * (map_size - 1) - x_0_index; // This value is used for single point collision
		if (stair_adjustment_value_X_0 > 0.5) {
			x_0_index++;
			stair_adjustment_value_X_0 = stair_adjustment_value_X_0 * (-1);
		}
			
		

		int x_1_index = x_1 * (map_size - 1);
		float stair_adjustment_value_X_1 = x_1 * (map_size - 1) - x_1_index;
		if (stair_adjustment_value_X_1 > 0.5) {
			x_1_index++;
			stair_adjustment_value_X_1 = stair_adjustment_value_X_1 * (-1);
		}



		int y_0_index = y_0 * (map_size - 1) * -1;
		float stair_adjustment_value_Y_0 = y_0 * (map_size - 1) * -1 - y_0_index;
		if (stair_adjustment_value_Y_0 > 0.5)
			y_0_index++;

		int y_1_index = y_1 * (map_size - 1) * -1;
		float stair_adjustment_value_Y_1 = y_1 * (map_size - 1) * -1 - y_1_index;
		if (stair_adjustment_value_Y_1 > 0.5)
			y_1_index++;


		// | 4 | Get an interval of map points to test against each object point.
		int x_min = __max(x_0_index - radius_constant, 0);
		x_min = __min(x_min, map_size - 1);
		int x_max = __min(x_0_index + 2 * in_object_ptr->GetScale().x + radius_constant, map_size - 1);
		x_max = __max(x_max, 0);

		int y_min = __max(y_0_index - radius_constant, 0);
		y_min = __min(y_min, map_size - 1);
		int y_max = __min(y_0_index + 2 * in_object_ptr->GetScale().y + radius_constant, map_size - 1);
		y_max = __max(y_max, 0);

		//std::cout << "xmin: " << x_min << ", xmax= " << x_max << ", ymin: " << y_min << ", ymax: " << y_max << std::endl;

		// | 5 | Check each point in the interval with a height, with each point in the object.
		for (int i = x_min; i < x_max; i++)
		{
			for (int j = y_min; j < y_max; j++)
			{
				//						y  x
				if ((*map_height_list_)[j][i] > 100.0f)
				{
					if (x_0_index == i && y_0_index == j)
					{
						collision_0 = true;
					}
					if (x_1_index == i && y_0_index == j)
					{
						collision_1 = true;
					}
					if (x_1_index == i && y_1_index == j)
					{
						collision_2 = true;
					}
					if (x_0_index == i && y_1_index == j)
					{
						collision_3 = true;
					}
				}
			}
		}

		// | 6 | Finally do things in a switch case if colliding.
		bool doublecollision = false;
		if (collision_0 && collision_1)	// Bot collision
		{
			//object_pos.y += 0.2f;
			object_pos.y = y_0_index * (-1) + in_object_ptr->GetScale().y * 1.25;
			//std::cout << "Bot Collision" << std::endl;
			doublecollision = true;
			in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
			//in_object_ptr->SetAirborne(false);
		}

		if (collision_0 && collision_3)	// Left collision
		{
			object_pos.x = x_0_index + in_object_ptr->GetScale().x * 1.25;
			//std::cout << "Left Collision" << std::endl;
			doublecollision = true;
			in_object_ptr->SetVelocityVec(glm::vec3(0.0f, in_object_ptr->GetVelocityVec().y, in_object_ptr->GetVelocityVec().z));
		}

		if (collision_1 && collision_2)	// Right collision
		{
			object_pos.x = x_1_index - in_object_ptr->GetScale().x * 1.25;
			//std::cout << "Right Collision" << std::endl;
			doublecollision = true;
			in_object_ptr->SetVelocityVec(glm::vec3(0.0f, in_object_ptr->GetVelocityVec().y, in_object_ptr->GetVelocityVec().z));
		}

		if (collision_3 && collision_2)	// Top collision
		{
			object_pos.y = y_1_index * (-1) - in_object_ptr->GetScale().y * 1.25;
			//std::cout << "Top Collision" << std::endl;
			doublecollision = true;
			in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
		}

		if (!doublecollision)
		{
			glm::vec3 obj_velocity = in_object_ptr->GetVelocityVec();
			if (collision_0)
			{
				float delta_x = x_0_index - object_pos.x - in_object_ptr->GetScale().x;
				object_pos.y = y_0_index * (-1) + in_object_ptr->GetScale().y - delta_x / 7.5f;

				//							  y			   x
				if ((*map_height_list_)[y_0_index - 1][x_0_index] <= 100.0f)
				{
					in_object_ptr->SetVelocityVec(glm::vec3(obj_velocity.x, 0.0f, obj_velocity.z));
				}
				else
				{
					in_object_ptr->SetVelocityVec(glm::vec3(0.0f, 0.0f, obj_velocity.z));
				}
				
				if (obj_velocity.y <= 0)
				{
					in_object_ptr->SetVelocityVec(glm::vec3(obj_velocity.x, 0.0f, obj_velocity.z));
				}

				//in_object_ptr->SetAirborne(false);

				//std::cout << "0 Collision" << std::endl;
			}
			else if (collision_1)
			{
				float delta_x = x_1_index - object_pos.x - in_object_ptr->GetScale().x;
				object_pos.y = y_0_index * (-1) + in_object_ptr->GetScale().y + delta_x / 7.5f;

				//							  y			   x
				if ((*map_height_list_)[y_0_index - 1][x_0_index] <= 100.0f)
				{
					in_object_ptr->SetVelocityVec(glm::vec3(obj_velocity.x, 0.0f, obj_velocity.z));
				}
				else
				{
					in_object_ptr->SetVelocityVec(glm::vec3(0.0f, 0.0f, obj_velocity.z));
				}

				if (obj_velocity.y <= 0)
				{
					in_object_ptr->SetVelocityVec(glm::vec3(obj_velocity.x, 0.0f, obj_velocity.z));
				}
				
				//in_object_ptr->SetAirborne(false);

				//std::cout << "1 Collision" << std::endl;
			}
			else if (collision_2)
			{
				object_pos.x -= 0.2;
				object_pos.y -= 0.2;
				//std::cout << "2 Collision" << std::endl;
				in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
				in_object_ptr->SetVelocityVec(glm::vec3(0.0f, in_object_ptr->GetVelocityVec().y, in_object_ptr->GetVelocityVec().z));
			}
			else if (collision_3)
			{
				object_pos.x += 0.2;
				object_pos.y -= 0.2;
				//std::cout << "3 Collision" << std::endl;
				in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
				in_object_ptr->SetVelocityVec(glm::vec3(0.0f, in_object_ptr->GetVelocityVec().y, in_object_ptr->GetVelocityVec().z));
			}
		}
		
	}
	

	//TEMP: DON'T LET AN OBJECT OUTSIDE THE MAP
	//in_object_ptr->SetAirborne(true);

	if (object_pos.y < -1 * map_size + 1) {
		object_pos.y = -1 * map_size + 1;
		glm::vec3 grounded_velocity = in_object_ptr->GetVelocityVec();
		grounded_velocity.y = 0.0f;
		in_object_ptr->SetVelocityVec(grounded_velocity);
		in_object_ptr->SetAirborne(false);
	}
	if (object_pos.x < 0) {
		object_pos.x = 0;
		glm::vec3 grounded_velocity = in_object_ptr->GetVelocityVec();
		grounded_velocity.x = 0.0f;
		in_object_ptr->SetVelocityVec(grounded_velocity);
		//in_object_ptr->object_metadata_.airborne = false;
		in_object_ptr->SetAirborne(false);
	}
	if (object_pos.x > map_size * map_size + 1) {
		object_pos.x = map_size * map_size + 1;
		glm::vec3 grounded_velocity = in_object_ptr->GetVelocityVec();
		grounded_velocity.x = 0.0f;
		in_object_ptr->SetVelocityVec(grounded_velocity);
		//in_object_ptr->object_metadata_.airborne = false;
		in_object_ptr->SetAirborne(false);
	}



	//Set the object's new position
	in_object_ptr->SetPosition(object_pos.x, object_pos.y, object_pos.z);

}

//Public---------------------------------------------------
PhysicsEngine::PhysicsEngine(std::vector<std::vector<float>>* in_map_height_list) {
	this->gravitational_acceleration_	= GRAVITATIONAL_ACCELERATION;
	this->object_max_velocity_			= OBJECT_MAX_VELOCITY;
	this->object_min_velocity_			= OBJECT_MIN_VELOCITY;
	this->object_ground_loss_ratio_		= (float)OBJECT_GROUND_LOSS_RATIO;
	this->object_air_loss_ratio_		= OBJECT_AIR_LOSS_RATIO;
	this->map_height_list_ = in_map_height_list;

	/*for (int j = (*map_height_list_->begin()); j < (*map_height_list_->end()); j++) {
		for (int i = (*map_height_list_->begin()); i < (*map_height_list_->end()); i++) {}
	}*/
}

PhysicsEngine::~PhysicsEngine() {

}

void PhysicsEngine::ApplyPhysics(float& in_deltatime, std::vector<ObjectClass*>& in_object_ptr_vector) {
	
	//Loop through all objects in vector
	for (unsigned int i = 0; i < in_object_ptr_vector.size(); i++) {
		this->UpdateVelocity(in_deltatime, in_object_ptr_vector.at(i));
		this->UpdatePosition(in_deltatime, in_object_ptr_vector.at(i));
	}


}