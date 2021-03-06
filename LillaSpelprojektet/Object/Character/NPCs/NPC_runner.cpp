#include "NPC_runner.h"
#include <stdlib.h>  	//Has abs()
#include <iostream> // just for debug

NPCRunner::NPCRunner(glm::vec3 start_pos, ObjectID id)
	: NPC(
		start_pos, 
		id,
		(int)GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_HP"),
		(int)GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_ATK")
	) {

	knockback_ = glm::vec2(GlobalSettings::Access()->ValueOf("NPC_KNOCKBACK_X"),
		GlobalSettings::Access()->ValueOf("NPC_KNOCKBACK_Y"));
	time_to_next_move_ = 0.0f;
	next_move_index_ = 0;
	aggro_range_ = GlobalSettings::Access()->ValueOf("NPC_AGGRO_RANGE");
	jump_velocity_ = GlobalSettings::Access()->ValueOf("NPC_RUNNER_JUMP_VELOCITY");
	aggro_speed_ = GlobalSettings::Access()->ValueOf("NPC_RUNNER_AGGRO_SPEED");
	float scale = GlobalSettings::Access()->ValueOf("NPC_RUNNER_SCALE");
	float hp = GlobalSettings::Access()->ValueOf("NPC_RUNNER_START_HP");
	//Set the base scale of this type of unit
	//this->SetScale(GlobalSettings::Access()->ValueOf("NPC_RUNNER_SCALE"));

	if (id == OBJECT_ID_FIRE_AI) {
		SetScale(1.5f);
		SetOffsets(1.5f, 1.5f);
		aggro_speed_ = 4000;
	}
	else if (id == OBJECT_ID_ICE_AI) {
		SetScale(scale);
		SetOffsets(scale, scale);
		aggro_range_ = 200;
		jump_velocity_ = 200;
	}
	else if (id == OBJECT_ID_WOOD_AI) {
		SetScale(4.0f);
		SetOffsets(4.0f, 4.0f);
		aggro_speed_ = 2300;
		SetMaxHealth((int)(hp * 3));
		SetCurrentHealth((int)(hp * 3));
	}
	else { // default 
		SetScale(scale);
		SetOffsets(scale, scale);
	}

	
	

}

NPCRunner::~NPCRunner() {
}

void NPCRunner::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos) {
	
	//If we are in the process of dying, return
	if (this->ExecuteDeath(in_deltatime)) { return; }

	glm::vec3 temp_position = GetPosition();
	glm::vec3 temp_velocity = GetVelocityVec();

	float idle_speed = GlobalSettings::Access()->ValueOf("NPC_RUNNER_IDLE_SPEED");

	static float private_time = 0.0f;
	
	float stun_duration = 0.5f;


	int health = GetCurrentHealth();
	stun_timer_ += in_deltatime;
	if (health_last_frame_ > health) {
		stunned_ = true;
		stun_timer_ = 0;
	}
	health_last_frame_ = health;

	if (!stunned_) {
		float length_to_player_x = (in_player_pos.x - temp_position.x);
		float length_to_player_y = (in_player_pos.y - temp_position.y);
		if (abs(length_to_player_x) < aggro_range_ && abs(length_to_player_y) < aggro_range_) {
			state_ = NPC_STATE_AGGRO;
		}
		else {
			if (state_ == NPC_STATE_AGGRO) {
				private_time = 3.0f;
				next_move_index_ = 0;
			}
			state_ = NPC_STATE_IDLE;
		}

		if (state_ == NPC_STATE_IDLE) {
			private_time += (float)in_deltatime;

			if (private_time > time_to_next_move_) {
				private_time = 0;
				next_move_index_ = GlobalSettings::GetRandomInt() % 3;
				time_to_next_move_ = GlobalSettings::GetRandomInt() % 10 + 2.0f;
			}

			switch (next_move_index_) {
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
		else if (state_ == NPC_STATE_AGGRO) {
			if (in_player_pos.x < temp_position.x) {
				SetVelocityVec({ -aggro_speed_ * in_deltatime ,temp_velocity.y , temp_velocity.z });
			}
			else if (in_player_pos.x > temp_position.x) {
				SetVelocityVec({ aggro_speed_ * in_deltatime ,temp_velocity.y , temp_velocity.z });
			}

			temp_velocity = GetVelocityVec();

			if (in_player_pos.y - temp_position.y > 10 && !IsAirborne()) {
				SetVelocityVec({ temp_velocity.x , jump_velocity_ , temp_velocity.z });
				SetAirborne(true);
			}
		}
	}
	else if (stun_timer_ > stun_duration) {
		stunned_ = false;
	}

	if (GetVelocityVec().x > 0) {
		TurnRight(in_deltatime);
	}
	else if (GetVelocityVec().x < 0) {
		TurnLeft(in_deltatime);
	}


}

bool NPCRunner::Attack(Character& in_target) {
	bool return_value = false;
	if (this->NPC::Attack(in_target)) {
		//Knockback player
		in_target.TakeDamage(this->GetAttackPower());
		if (in_target.GetPosition().x < GetPosition().x) {
			in_target.SetVelocityVec(glm::vec3(-1 * knockback_.x, knockback_.y, 0.0f));
			return_value = true;
		}
		else if (in_target.GetPosition().x > GetPosition().x) {
			in_target.SetVelocityVec(glm::vec3(knockback_.x, knockback_.y, 0.0f));
			return_value = true;
		}
	}
	return return_value;
}

void NPCRunner::SetAggroRange(int aggro_range) {
	aggro_range_ = (float)aggro_range;
}

void NPCRunner::SetJumpVelocity(float jump_velocity) {
	jump_velocity_ = jump_velocity;
}

void NPCRunner::SetAggroVelocity(float aggro_velocity) {
	aggro_speed_ = aggro_velocity;
}
