#include "NPC_runner.h"
#include <stdlib.h>  	//Has abs()
#include <iostream> // just for debug

NPCRunner::NPCRunner(glm::vec3 start_pos, ObjectID id) : NPC(start_pos, id) {
	time_to_next_move_ = 0.0f;
	next_move_index_ = 0;
	aggro_range_ = GlobalSettings::Access()->ValueOf("NPC_AGGRO_RANGE");
}

void NPCRunner::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos)
{
	glm::vec3 pos = GetPosition();
	glm::vec3 v = GetVelocityVec();

	static float private_time = 0.0f;


	float length_to_player_x = (in_player_pos.x - pos.x);
	float length_to_player_y = (in_player_pos.y - pos.y);
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
			SetVelocityVec({ 1000.0f * in_deltatime ,v.y , v.z });
			break;
		case 2:
			SetVelocityVec({ -1000.0f * in_deltatime ,v.y , v.z });
			break;
		default:
			break;
		} // switch
	} // if Idle
	else if (state_ == NPC_STATE_AGGRO)
	{
		if (in_player_pos.x < pos.x)
		{
			SetVelocityVec({ -2500.0f * in_deltatime ,v.y , v.z });
		}
		else if (in_player_pos.x > pos.x)
		{
			SetVelocityVec({ 2500.0f * in_deltatime ,v.y , v.z });
		}

		v = GetVelocityVec();

		if (in_player_pos.y - pos.y > 10 && !IsAirborne())
		{
			SetVelocityVec({ v.x , 100.0f , v.z });
			SetAirborne(true);
		}
	}
}
