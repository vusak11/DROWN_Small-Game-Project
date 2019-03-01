#include "physics_engine.h"
#include <iostream>
#include "../../global_settings.h"
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

	//Get the object's position
	glm::vec3 object_pos = in_object_ptr->GetPosition();

	//Displace the object using its velocity during deltatime
	object_pos += in_object_ptr->GetVelocityVec()*in_deltatime;

	//With the new location, check collision and return a new valid position. 
	object_pos = CheckCollision(in_object_ptr, object_pos);

	//DON'T LET AN OBJECT OUTSIDE THE MAP
	int map_size = GlobalSettings::Access()->ValueOf("MAP_SIZE");
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

glm::vec3 PhysicsEngine::CheckCollision(ObjectClass *& in_object_ptr, glm::vec3 new_pos) {

	int map_size = GlobalSettings::Access()->ValueOf("MAP_SIZE");	// OBS this variable needs to be collected from the map
	int radius_constant = 6;
	bool print = false;
	glm::vec3 object_pos = new_pos;

	// Reset collosion flags
	bool collision_0 = false;
	bool collision_1 = false;
	bool collision_2 = false;
	bool collision_3 = false;

	in_object_ptr->SetAirborne(true);

	HitBox hitbox;
	hitbox.UpdateHitbox(object_pos, in_object_ptr->GetScale().x, in_object_ptr->GetScale().y);
	BoxPoints points = hitbox.GetPoints();

	// Map each hitbox point to a map tile index
	int x_0_index = points.bottomLeft.x;
	float stair_adjustment_value_X_0 = points.bottomLeft.x - x_0_index; // This value is used for single point collision
	if (stair_adjustment_value_X_0 > 0.5) {
		x_0_index++;
		stair_adjustment_value_X_0 = (1 - stair_adjustment_value_X_0) * (-1);
	}

	int x_1_index = points.bottomRight.x;
	float stair_adjustment_value_X_1 = points.bottomRight.x - x_1_index;
	if (stair_adjustment_value_X_1 > 0.5) {
		x_1_index++;
		stair_adjustment_value_X_1 = (1 - stair_adjustment_value_X_1) * (-1);
	}

	int y_0_index = points.bottomLeft.y * -1;
	float stair_adjustment_value_Y_0 = points.bottomLeft.y * -1 - y_0_index;
	if (stair_adjustment_value_Y_0 > 0.5)
		y_0_index++;

	int y_1_index = points.TopLeft.y * -1;
	float stair_adjustment_value_Y_1 = points.TopLeft.y * -1 - y_1_index;
	if (stair_adjustment_value_Y_1 > 0.5)
		y_1_index++;

	if ((*map_height_list_)[y_0_index][x_0_index] > 100.0f)
	{
		collision_0 = true;
	}
	if ((*map_height_list_)[y_0_index][x_1_index] > 100.0f)
	{
		collision_1 = true;
	}
	if ((*map_height_list_)[y_1_index][x_1_index] > 100.0f)
	{
		collision_2 = true;
	}
	if ((*map_height_list_)[y_1_index][x_0_index] > 100.0f)
	{
		collision_3 = true;
	}

	// | 6 | Finally do things in a switch case if colliding.
	bool doublecollision = false;
	bool fourColl = false;

	if (collision_0 && collision_1 && collision_2 && collision_3)
	{
		// THIS FUNCTION DOES NOT WORK AT ALL TIMES
		// The angle gives wrong angle ond wont be putting the object back if stuck in right wall.
		if (print)
		{
			std::cout << "All 4 Collision" << std::endl;
		}
		fourColl = true;
		glm::vec3 obj_pos = in_object_ptr->GetPosition();
		glm::vec3 diff = glm::normalize(obj_pos - new_pos);
		glm::vec3 baseVec = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
		float angle = glm::angle(diff, baseVec);
		float pi = GlobalSettings::Access()->ValueOf("PI");

		if (angle < pi/2.0f)// apply left
		{
			bool free = false;

			int free_index = x_0_index;	// The index of the surface
			for (int f = 1; f < map_size - x_0_index; f++)
			{
				if ((*map_height_list_)[y_0_index][x_0_index + f] <= 100.0f)
				{
					free_index = x_0_index + f;
					free = true;
				}
				if ((*map_height_list_)[y_1_index][x_1_index + f] <= 100.0f)
				{
					free_index = x_0_index + f;
					free = true;
				}
				if (free)
				{
					f = map_size - x_0_index;
				}
			}

			object_pos.x = free_index + in_object_ptr->GetScale().x;

			if (in_object_ptr->GetVelocityVec().x < 0)
			{
				in_object_ptr->SetVelocityVec(glm::vec3(0.0f, in_object_ptr->GetVelocityVec().y, in_object_ptr->GetVelocityVec().z));
			}

			if (print)
			{
				std::cout << "Left Collision" << std::endl;
			}
		}
		else if (angle < pi && angle >= pi / 2.0f) // apply bot
		{
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
		}
		else if (angle < (3.0f * pi) / 2.0f  && angle >= pi) // apply right
		{
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
		else // apply top
		{
		float side = 0.0f;
		bool free = false;

		int free_index = y_1_index + 1;	// The index of the surface
		for (int f = 1; f < map_size - y_1_index - 1; f++)
		{
			if ((*map_height_list_)[y_1_index + f][x_0_index] <= 100.0f)
			{
				free_index = y_1_index + f;
				free = true;
				side += 0.25f;
			}
			if ((*map_height_list_)[y_1_index + f][x_1_index] <= 100.0f)
			{
				free_index = y_1_index + f;
				side += 0.25f;
				free = true;
			}
			if (free)
			{
				f = map_size - y_1_index - 1;
			}
		}

		object_pos.y = free_index * (-1) + side - in_object_ptr->GetScale().y;

		if (in_object_ptr->GetVelocityVec().y > 0)
		{
			in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
		}
		}
		/*glm::vec3 pos = {
			obj_pos.x + diff.x * 0.75f,
			obj_pos.y + diff.y * 0.75f,
			0.0f };
		object_pos = CheckCollision(in_object_ptr, pos);*/
	}
	else if ((collision_1 && collision_2 && collision_3) || (collision_0 && collision_2 && collision_3))
	{
		if (print)
		{
			std::cout << "Top and Side Collision" << std::endl;
		}
		float side = 0.0f;
		bool free = false;

		int free_index = y_1_index + 1;	// The index of the surface
		for (int f = 1; f < map_size - y_1_index - 1; f++)
		{
			if ((*map_height_list_)[y_1_index + f][x_0_index] <= 100.0f)
			{
				free_index = y_1_index + f;
				free = true;
				side += 0.25f;
			}
			if ((*map_height_list_)[y_1_index + f][x_1_index] <= 100.0f)
			{
				free_index = y_1_index + f;
				side += 0.25f;
				free = true;
			}
			if (free)
			{
				f = map_size - y_1_index - 1;
			}
		}

		object_pos.y = free_index * (-1) + side - in_object_ptr->GetScale().y;
		in_object_ptr->SetVelocityVec(glm::vec3(0.0f, 0.0f, in_object_ptr->GetVelocityVec().z));
		
		if (collision_0) // increase x a little
		{
			object_pos.x += 0.2;
		}
		else
		{
			object_pos.x -= 0.2;
		}

		object_pos = CheckCollision(in_object_ptr, object_pos);
	}
	else if ((collision_0 && collision_1 && collision_3) || (collision_0 && collision_1 && collision_2))
	{
		if (print)
		{
			std::cout << "Bot and side Collision" << std::endl;
		}
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

		in_object_ptr->SetAirborne(false);
		object_pos.y = free_index * (-1) - side + in_object_ptr->GetScale().y;
		in_object_ptr->SetVelocityVec(glm::vec3(0.0f, 0.0f, in_object_ptr->GetVelocityVec().z));
		
		if (collision_3) // increase x a little
		{
			object_pos.x += 0.2;
		}
		else
		{
			object_pos.x -= 0.2;
		}

		object_pos = CheckCollision(in_object_ptr, object_pos);
	}
	
	if (!fourColl)
	{
		// Bot collision
		if (collision_0 && collision_1) // Bot collision
		{
			if (print)
			{
				std::cout << "Bot Collision" << std::endl;
			}

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
		}
		// Left collision
		if (collision_0 && collision_3)	// Left collision
		{

			doublecollision = true;

			bool free = false;

			int free_index = x_0_index;	// The index of the surface
			for (int f = 1; f < map_size - x_0_index; f++)
			{
				if ((*map_height_list_)[y_0_index][x_0_index + f] <= 100.0f)
				{
					free_index = x_0_index + f;
					free = true;
				}
				if ((*map_height_list_)[y_1_index][x_1_index + f] <= 100.0f)
				{
					free_index = x_0_index + f;
					free = true;
				}
				if (free)
				{
					f = map_size - x_0_index;
				}
			}

			object_pos.x = free_index + in_object_ptr->GetScale().x;

			if (in_object_ptr->GetVelocityVec().x < 0)
			{
				in_object_ptr->SetVelocityVec(glm::vec3(0.0f, in_object_ptr->GetVelocityVec().y, in_object_ptr->GetVelocityVec().z));
			}

			if (print)
			{
				std::cout << "Left Collision" << std::endl;
			}
		}
		// Right collision
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
		// Top collision
		if (collision_3 && collision_2)	// Top collision
		{
			if (print)
			{
				std::cout << "Top Collision" << std::endl;
			}

			doublecollision = true;


			float side = 0.0f;
			bool free = false;

			int free_index = y_1_index + 1;	// The index of the surface
			for (int f = 1; f < map_size - y_1_index - 1; f++)
			{
				if ((*map_height_list_)[y_1_index + f][x_0_index] <= 100.0f)
				{
					free_index = y_1_index + f;
					free = true;
					side += 0.25f;
				}
				if ((*map_height_list_)[y_1_index + f][x_1_index] <= 100.0f)
				{
					free_index = y_1_index + f;
					side += 0.25f;
					free = true;
				}
				if (free)
				{
					f = map_size - y_1_index - 1;
				}
			}

			object_pos.y = free_index * (-1) + side - in_object_ptr->GetScale().y;

			if (in_object_ptr->GetVelocityVec().y > 0)
			{
				in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
			}













			//int free_index = y_1_index - 1;	// The index of the surface
			//int iterations = map_size - y_1_index - 1;
			//for (int f = 1; f < iterations; f++)
			//{
			//	if ((*map_height_list_)[y_1_index + f][x_0_index] <= 100.0f)
			//	{
			//		free_index = y_0_index + f;
			//		//std::cout << "f: " << f << std::endl;
			//		f = y_0_index - 1;
			//		continue;
			//	}
			//	if ((*map_height_list_)[y_1_index + f][x_1_index] <= 100.0f)
			//	{
			//		free_index = y_0_index + f;
			//		//std::cout << "f: " << f << std::endl;
			//		f = y_0_index - 1;
			//		continue;
			//	}
			//}

			//object_pos.y = free_index * (-1) + 0.5f - in_object_ptr->GetScale().y;

			//if (in_object_ptr->GetVelocityVec().y > 0)
			//{
			//	in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
			//}
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

				bool stair_flag = true;
				for (int f = 1; f < in_object_ptr->GetScale().x; f++)
				{
					if ((*map_height_list_)[free_index + 1][x_0_index + f] > 100.0f)
					{
						stair_flag = false;
						continue;
					}
				}

				if (stair_flag)
				{
					object_pos.y = free_index * (-1) - stair_adjustment_value_X_0 + (in_object_ptr->GetScale().y / 2);
				}
				else
				{
					object_pos.y = free_index * (-1) - 0.5f + (in_object_ptr->GetScale().y);
				}


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
						f = y_0_index - 1;
						continue;
					}
				}

				bool stair_flag = true;
				for (int f = 1; f < in_object_ptr->GetScale().x; f++)
				{
					if ((*map_height_list_)[free_index + 1][x_1_index - f] > 100.0f)
					{
						stair_flag = false;
						continue;
					}
				}

				if (stair_flag)
				{
					object_pos.y = free_index * (-1) + stair_adjustment_value_X_1 + (in_object_ptr->GetScale().y / 2);
				}
				else
				{
					object_pos.y = free_index * (-1) + 0.5f + (in_object_ptr->GetScale().y / 2);
				}








				//int free_index = y_0_index;	// The index of the surface
				//for (int f = 1; f < y_0_index - 1; f++)
				//{
				//	if ((*map_height_list_)[y_0_index - f][x_1_index] <= 100.0f)
				//	{
				//		free_index = y_0_index - f;
				//		//std::cout << "f: " << f << std::endl;
				//		f = y_0_index - 1;
				//		continue;
				//	}
				//}

				//object_pos.y = free_index * (-1) + stair_adjustment_value_X_1 + (in_object_ptr->GetScale().y / 2);

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
				//int free_index = y_1_index;	// The index of the surface
				//int iterations = __min(map_size - y_0_index - 1, map_size / 5);
				//for (int f = 1; f < iterations; f++)
				//{
				//	if ((*map_height_list_)[y_1_index + f][x_1_index] <= 100.0f)
				//	{
				//		free_index = y_1_index + f;
				//		//std::cout << "f: " << f << std::endl;
				//		f = y_0_index - 1;
				//		continue;
				//	}
				//}

				//object_pos.y = free_index * (-1) - stair_adjustment_value_X_1 - (in_object_ptr->GetScale().y);

				//if (in_object_ptr->GetVelocityVec().y > 0)
				//{
				//	in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
				//}

				if (print)
				{
					std::cout << "2 Collision" << std::endl;
				}
			}
			else if (collision_3)
			{
				//int free_index = y_1_index;	// The index of the surface
				//int iterations = __min(map_size - y_0_index - 1, map_size / 5);
				//for (int f = 1; f < iterations; f++)
				//{
				//	if ((*map_height_list_)[y_1_index + f][x_0_index] <= 100.0f)
				//	{
				//		free_index = y_1_index + f;
				//		//std::cout << "f: " << f << std::endl;
				//		f = y_0_index - 1;
				//		continue;
				//	}
				//}

				//object_pos.y = free_index * (-1) - stair_adjustment_value_X_0 - (in_object_ptr->GetScale().y);

				//if (in_object_ptr->GetVelocityVec().y > 0)
				//{
				//	in_object_ptr->SetVelocityVec(glm::vec3(in_object_ptr->GetVelocityVec().x, 0.0f, in_object_ptr->GetVelocityVec().z));
				//}

				if (print)
				{
					std::cout << "3 Collision" << std::endl;
				}
			}
		}
	}
	
	return object_pos;
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
	}


}