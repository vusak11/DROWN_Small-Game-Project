#include "NPC_ghost.h"

NPCGhost::NPCGhost(glm::vec3 start_pos) : NPC(
	start_pos,
	ObjectID::OBJECT_ID_GHOST,
	(int)GlobalSettings::Access()->ValueOf("NPC_GHOST_START_HP"),
	(int)GlobalSettings::Access()->ValueOf("NPC_GHOST_START_ATK")
) {

	knockback_ = glm::vec2(GlobalSettings::Access()->ValueOf("NPC_KNOCKBACK_X"),
		GlobalSettings::Access()->ValueOf("NPC_KNOCKBACK_Y"));
	time_to_next_move_ = 0.0f;
	next_move_index_ = 0;
	aggro_range_ = 300;
	health_last_frame_ = GetCurrentHealth();
	SetUsingPhysics(false);
	SetScale(3,3,3);
	SetOffsets(3,3);
}

NPCGhost::~NPCGhost() {
}

void NPCGhost::ExecuteAI(float in_deltatime, glm::vec3 in_player_pos) {

	//If we are in the process of dying, return
	if (this->ExecuteDeath(in_deltatime)) { return; }

	glm::vec3 temp_position = GetPosition();
	glm::vec3 temp_velocity = GetVelocityVec();
	float aggro_speed = GlobalSettings::Access()->ValueOf("NPC_GHOST_AGGRO_SPEED");
	
	float length_to_player_x = (in_player_pos.x - temp_position.x);
	float length_to_player_y = (in_player_pos.y - temp_position.y);
	glm::vec3 dir = in_player_pos - temp_position; // dir = direction
	float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
	dir = dir / length;
	dir *= aggro_speed;

	SetVelocityVec(dir);

}

bool NPCGhost::Attack(Character& in_target) {
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

int NPCGhost::GetHealthLastFrame() const {
	return health_last_frame_;
}

void NPCGhost::SetAggroRange(int aggro_range) {
	this->aggro_range_ = aggro_range;
}
