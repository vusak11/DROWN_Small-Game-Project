#include "NPC_runner.h"
#include <stdlib.h>  	//Has abs()
#include <iostream> // just for debug

NPCRunner::NPCRunner(glm::vec3 start_pos, ObjectID id)
	: NPC(
		start_pos, 
		id,
		GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_HP"),
		GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_ATK")
	) {

	knockback_ = glm::vec2(GlobalSettings::Access()->ValueOf("NPC_KNOCKBACK_X"),
		GlobalSettings::Access()->ValueOf("NPC_KNOCKBACK_Y"));
	time_to_next_move_ = 0.0f;
	next_move_index_ = 0;
	aggro_range_ = GlobalSettings::Access()->ValueOf("NPC_AGGRO_RANGE");
	jump_velocity_ = GlobalSettings::Access()->ValueOf("NPC_RUNNER_JUMP_VELOCITY");
	aggro_speed_ = GlobalSettings::Access()->ValueOf("NPC_RUNNER_AGGRO_SPEED");
}

NPCRunner::~NPCRunner() {
}

void NPCRunner::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos)
{
	glm::vec3 temp_position = GetPosition();
	glm::vec3 temp_velocity = GetVelocityVec();

	float idle_speed = GlobalSettings::Access()->ValueOf("NPC_RUNNER_IDLE_SPEED");

	static float private_time = 0.0f;
	
	float stun_duration = 0.6;


	int health = GetCurrentHealth();
	stun_timer += in_deltatime;
	if (health_last_frame_ > health)
	{
		stunned_ = true;
		stun_timer = 0;
	}
	health_last_frame_ = health;

	if (!stunned_)
	{
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
				SetVelocityVec({ -aggro_speed_ * in_deltatime ,temp_velocity.y , temp_velocity.z });
			}
			else if (in_player_pos.x > temp_position.x)
			{
				SetVelocityVec({ aggro_speed_ * in_deltatime ,temp_velocity.y , temp_velocity.z });
			}

			temp_velocity = GetVelocityVec();

			if (in_player_pos.y - temp_position.y > 10 && !IsAirborne())
			{
				SetVelocityVec({ temp_velocity.x , jump_velocity_ , temp_velocity.z });
				SetAirborne(true);
			}
		}
	}
	else if (stun_timer > stun_duration)
	{
		stunned_ = false;
	}

	if (GetVelocityVec().x > 0)
	{
		TurnRight(in_deltatime);
	}
	else if (GetVelocityVec().x < 0)
	{
		TurnLeft(in_deltatime);
	}


	
}

bool NPCRunner::Attack(Character& in_target) {
	if (this->NPC::Attack(in_target)) {
		//Knockback player
		in_target.TakeDamage(this->GetAttackPower());
		if (in_target.GetPosition().x < GetPosition().x)
			in_target.SetVelocityVec(glm::vec3(-1 * knockback_.x, knockback_.y, 0.0f));
		else if (in_target.GetPosition().x > GetPosition().x)
			in_target.SetVelocityVec(glm::vec3(knockback_.x, knockback_.y, 0.0f));
	}
}

void NPCRunner::SetAggroRange(int aggro_range) {
	aggro_range_ = aggro_range;
}

void NPCRunner::SetJumpVelocity(float jump_velocity) {
	jump_velocity_ = jump_velocity;
}

void NPCRunner::SetAggroVelocity(float aggro_velocity) {
	aggro_speed_ = aggro_velocity;
}
