#include "NPC_runner.h"
#include <stdlib.h>  	//Has abs()
#include <iostream> // just for debug

NPCRunner::NPCRunner(glm::vec3 start_pos, ObjectID id) : NPC(start_pos, id) {
	time_to_next_move_ = 0.0f;
	next_move_index_ = 0;
	aggro_range_ = GlobalSettings::Access()->ValueOf("NPC_AGGRO_RANGE");
}

NPCRunner::~NPCRunner() {
}

void NPCRunner::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos)
{
	glm::vec3 temp_position = GetPosition();
	glm::vec3 temp_velocity = GetVelocityVec();

	float idle_speed = GlobalSettings::Access()->ValueOf("NPC_RUNNER_IDLE_SPEED");
	float aggro_speed = GlobalSettings::Access()->ValueOf("NPC_RUNNER_AGGRO_SPEED");
	float jump_velocity = GlobalSettings::Access()->ValueOf("NPC_RUNNER_JUMP_VELOCITY");

	static float private_time = 0.0f;


	float length_to_player_x = (in_player_pos.x - temp_position.x);
	float length_to_player_y = (in_player_pos.y - temp_position.y);
	if (abs(length_to_player_x) < aggro_range_ && abs(length_to_player_y) < aggro_range_)
	{
		state_ = NPC_STATE_AGGRO;
	}
	else
	{
		if (state_ == NPC_STATE_AGGRO)
		{
			private_time = 3.0f;
			next_move_index_ = 0.0f;
		}
		state_ = NPC_STATE_IDLE;
	}

	if (state_ == NPC_STATE_IDLE)
	{
		private_time += (float)in_deltatime;

		if (private_time > time_to_next_move_)
		{
			private_time = 0;
			next_move_index_ = rand() % 3;
			time_to_next_move_ = rand() % 10 + 2;
		}

		switch (next_move_index_)
		{
		case 1:
			SetVelocityVec({ idle_speed * in_deltatime ,temp_velocity.y , temp_velocity.z });
			break;
		case 2:
			SetVelocityVec({ -idle_speed * in_deltatime ,temp_velocity.y , temp_velocity.z });
			break;
		default:
			break;
		} // switch
	} // if Idle
	else if (state_ == NPC_STATE_AGGRO)
	{
		if (in_player_pos.x < temp_position.x)
		{
			SetVelocityVec({ -aggro_speed * in_deltatime ,temp_velocity.y , temp_velocity.z });
		}
		else if (in_player_pos.x > temp_position.x)
		{
			SetVelocityVec({ aggro_speed * in_deltatime ,temp_velocity.y , temp_velocity.z });
		}

		temp_velocity = GetVelocityVec();

		if (in_player_pos.y - temp_position.y > 10 && !IsAirborne())
		{
			SetVelocityVec({ temp_velocity.x , jump_velocity , temp_velocity.z });
			SetAirborne(true);
		}
	}
}
