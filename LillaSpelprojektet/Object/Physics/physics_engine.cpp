#include "physics_engine.h"
#include <iostream>
#include "../../global_settings.h"
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
	float loss_ratio = (in_object_ptr->IsAirborne()) ?
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

	int map_size = GlobalSettings::Access()->ValueOf("MAP_SIZE");	// OBS this variable needs to be collected from the map
	int radius_constant = 6;
	bool print = true;
	// | 1 | Get new position from acceleration and velocity.

	//Get the object's position
	glm::vec3 object_pos = in_object_ptr->GetPosition();

	//Displace the object using its velocity during deltatime
	object_pos += in_object_ptr->GetVelocityVec()*in_deltatime;

	//std::cout << "pos x,y: " << (int)in_object_ptr->GetPosition().x << " " << (int)in_object_ptr->GetPosition().y
	//	<< " ,,, newPos x,y: " << (int)object_pos.x << " " << (int)object_pos.y << std::endl;

	//float test = in_object_ptr->GetPosition().y - object_pos.y;
	//std::cout << "test value: " << test << std::endl;


	// Reset collosion flags
	bool collision_0 = false;
	bool collision_1 = false;
	bool collision_2 = false;
	bool collision_3 = false;

	in_object_ptr->SetAirborne(true);

	HitBox hitbox;
	hitbox.UpdateHitbox(object_pos, in_object_ptr->GetScale().x, in_object_ptr->GetScale().y);
	BoxPoints points = hitbox.GetPoints();

	//int x_0_index = (int)points.bottomLeft.x;
	//float margin_x_0 = points.bottomLeft.x - x_0_index;
	//if (margin_x_0 >= 0.5f)
	//{
	//	x_0_index++;
	//}
	//else
	//{
	//	margin_x_0 *= -1;
	//}
	//int x_1_index = (int)points.bottomRight.x;
	//float margin_x_1 = points.bottomRight.x - x_1_index;
	//if (margin_x_1 >= 0.5f)
	//{
	//	x_1_index++;
	//}
	//else
	//{
	//	margin_x_1 *= -1;
	//}
	//int y_0_index = (int)points.TopLeft.y;
	//if (points.TopLeft.y - y_0_index >= 0.5f)
	//{
	//	y_0_index++;
	//}
	//int y_1_index = (int)points.TopRight.y;
	//if (points.TopRight.y - y_1_index >= 0.5f)
	//{
	//	y_1_index++;
	//}





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
		stair_adjustment_value_X_0 = (1-stair_adjustment_value_X_0) * (-1);
	}



	int x_1_index = x_1 * (map_size - 1);
	float stair_adjustment_value_X_1 = x_1 * (map_size - 1) - x_1_index;
	if (stair_adjustment_value_X_1 > 0.5) {
		x_1_index++;
		stair_adjustment_value_X_1 = (1-stair_adjustment_value_X_1) * (-1);
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


	//----------Bot collision----------Bot collision----------Bot collision----------//
	if (collision_0 && collision_1)
	{
		doublecollision = true;

		float side = 0.0f;
		bool free = false;

		int free_index = y_0_index;	// The index of the surface
		for (int f = 1; f < y_0_index - 1; f++)
		{
			if ((*map_height_list_)[y_0_index - f][x_0_index] <= 100.0f)
			{
				free_index = y_0_index - f;
				free = true;
				side += 0.25f;
			}
			if ((*map_height_list_)[y_0_index - f][x_1_index] <= 100.0f)
			{
				free_index = y_0_index - f;
				side += 0.25f;
				free = true;
			}
			if (free)
			{
				f = y_0_index - 1;
			}
		}

		object_pos.y = free_index * (-1) - side + in_object_ptr->GetScale().y;

		if (in_object_ptr->GetVelocityVec().y <= 0)
		{
			in_object_ptr->SetAirborne(false);
			in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
		}

		

		if (print)
		{
			std::cout << "Bot Collision" << std::endl;
		}
	}

	if (collision_0 && collision_3)	// Left collision
	{
		doublecollision = true;

		int free_index = x_0_index;	// The index of the surface
		for (int f = 1; f < x_1_index - 1; f++)
		{
			if ((*map_height_list_)[y_0_index][x_0_index + f] <= 100.0f)
			{
				free_index = x_0_index + f;
				//std::cout << "f: " << f << std::endl;
				f = x_1_index - 1;
				continue;
			}
			if ((*map_height_list_)[y_1_index][x_0_index + f] <= 100.0f)
			{
				free_index = x_0_index + f;
				//std::cout << "f: " << f << std::endl;
				f = x_1_index - 1;
				continue;
			}
		}

		object_pos.x = free_index - 0.5f + in_object_ptr->GetScale().x;

		if (in_object_ptr->GetVelocityVec().x < 0)
		{
			in_object_ptr->SetVelocityVec(glm::vec3(0.0f, in_object_ptr->GetVelocityVec().y, in_object_ptr->GetVelocityVec().z));
		}

		if (print)
		{
			std::cout << "Left Collision" << std::endl;
		}
	}

	if (collision_1 && collision_2)	// Right collision
	{
		doublecollision = true;

		int free_index = x_1_index;	// The index of the surface
		for (int f = 1; f < x_0_index - 1; f++)
		{
			if ((*map_height_list_)[y_0_index][x_1_index - f] <= 100.0f)
			{
				free_index = x_1_index - f;
				//std::cout << "f: " << f << std::endl;
				f = x_1_index - 1;
				continue;
			}
			if ((*map_height_list_)[y_1_index][x_1_index - f] <= 100.0f)
			{
				free_index = x_1_index - f;
				//std::cout << "f: " << f << std::endl;
				f = x_1_index - 1;
				continue;
			}
		}

		object_pos.x = free_index + 0.5f - in_object_ptr->GetScale().x;

		if (in_object_ptr->GetVelocityVec().x > 0)
		{
			in_object_ptr->SetVelocityVec(glm::vec3(0.0f, in_object_ptr->GetVelocityVec().y, in_object_ptr->GetVelocityVec().z));
		}

		if (print)
		{
			std::cout << "Right Collision" << std::endl;
		}
	}

	if (collision_3 && collision_2)	// Top collision
	{

		doublecollision = true;


		int free_index = y_1_index;	// The index of the surface
		int iterations = __min(map_size - y_0_index - 1, map_size / 5);
		for (int f = 1; f < iterations; f++)
		{
			if ((*map_height_list_)[y_0_index + f][x_0_index] <= 100.0f)
			{
				free_index = y_0_index + f;
				//std::cout << "f: " << f << std::endl;
				f = y_0_index - 1;
				continue;
			}
			if ((*map_height_list_)[y_0_index + f][x_1_index] <= 100.0f)
			{
				free_index = y_0_index + f;
				//std::cout << "f: " << f << std::endl;
				f = y_0_index - 1;
				continue;
			}
		}

		object_pos.y = free_index * (-1) - 0.5f + in_object_ptr->GetScale().y;

		if (in_object_ptr->GetVelocityVec().y > 0)
		{
			in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
		}





		if (print)
		{
			std::cout << "Top Collision" << std::endl;
		}
	}



	if (!doublecollision)
	{
		glm::vec3 obj_velocity = in_object_ptr->GetVelocityVec();
		if (collision_0)
		{

			int free_index = y_0_index;	// The index of the surface
			for (int f = 1; f < y_0_index - 1; f++)
			{
				if ((*map_height_list_)[y_0_index - f][x_0_index] <= 100.0f)
				{
					free_index = y_0_index - f;
					//std::cout << "f: " << f << std::endl;
					f = y_0_index - 1;
					continue;
				}
			}
			
			object_pos.y = free_index * (-1) - stair_adjustment_value_X_0 + (in_object_ptr->GetScale().y / 2);

			if (in_object_ptr->GetVelocityVec().y <= 0)
			{
				in_object_ptr->SetAirborne(false);
				in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
			}


			if (print)
			{
				std::cout << "0 Collision" << std::endl;
			}
		}
		else if (collision_1)
		{
			int free_index = y_0_index;	// The index of the surface
			for (int f = 1; f < y_0_index - 1; f++)
			{
				if ((*map_height_list_)[y_0_index - f][x_1_index] <= 100.0f)
				{
					free_index = y_0_index - f;
					//std::cout << "f: " << f << std::endl;
					f = y_0_index - 1;
					continue;
				}
			}

			object_pos.y = free_index * (-1) + stair_adjustment_value_X_1 + (in_object_ptr->GetScale().y / 2);

			if (in_object_ptr->GetVelocityVec().y <= 0)
			{
				in_object_ptr->SetAirborne(false);
				in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
			}

			
			if (print)
			{
				std::cout << "1 Collision" << std::endl;
			}
			
		}
		else if (collision_2)
		{
			int free_index = y_1_index;	// The index of the surface
			int iterations = __min(map_size - y_0_index - 1, map_size / 5);
			for (int f = 1; f < iterations; f++)
			{
				if ((*map_height_list_)[y_1_index + f][x_1_index] <= 100.0f)
				{
					free_index = y_1_index + f;
					//std::cout << "f: " << f << std::endl;
					f = y_0_index - 1;
					continue;
				}
			}

			object_pos.y = free_index * (-1) - stair_adjustment_value_X_1 - (in_object_ptr->GetScale().y);

			if (in_object_ptr->GetVelocityVec().y > 0)
			{
				in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
			}

			if (print)
			{
				std::cout << "2 Collision" << std::endl;
			}
		}
		else if (collision_3)
		{
			int free_index = y_1_index;	// The index of the surface
			int iterations = __min(map_size - y_0_index - 1, map_size / 5);
			for (int f = 1; f < iterations; f++)
			{
				if ((*map_height_list_)[y_1_index + f][x_0_index] <= 100.0f)
				{
					free_index = y_1_index + f;
					//std::cout << "f: " << f << std::endl;
					f = y_0_index - 1;
					continue;
				}
			}

			object_pos.y = free_index * (-1) - stair_adjustment_value_X_0 - (in_object_ptr->GetScale().y);

			if (in_object_ptr->GetVelocityVec().y > 0)
			{
				in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
			}

			if (print)
			{
				std::cout << "3 Collision" << std::endl;
			}
		}
	}




	//TEMP: DON'T LET AN OBJECT OUTSIDE THE MAP
	

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
	this->gravitational_acceleration_	= GlobalSettings::Access()->ValueOf("GRAVITATIONAL_ACCELERATION");
	this->object_max_velocity_			= GlobalSettings::Access()->ValueOf("OBJECT_MAX_VELOCITY");
	this->object_min_velocity_			= GlobalSettings::Access()->ValueOf("OBJECT_MIN_VELOCITY");
	this->object_ground_loss_ratio_		= GlobalSettings::Access()->ValueOf("OBJECT_GROUND_LOSS_RATIO");
	this->object_air_loss_ratio_		= GlobalSettings::Access()->ValueOf("OBJECT_AIR_LOSS_RATIO");
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
		//TBA(?): COLLISION DETECTION VS MAP
	}


}