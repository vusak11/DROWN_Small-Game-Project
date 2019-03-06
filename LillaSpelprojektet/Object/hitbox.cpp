#include "hitbox.h"



Hitbox::Hitbox() {
	position_ = glm::vec3(1.0f, 1.0f, 1.0f);
	x_offset_ = 1;
	y_offset_ = 1;
}

Hitbox::Hitbox(glm::vec3 position, float x_offset, float y_offset) {
	position_ = position;
	x_offset_ = x_offset;
	y_offset_ = y_offset;
}

Hitbox::~Hitbox() {
}

void Hitbox::UpdateHitbox(glm::vec3 position, float x_offset, float y_offset) {
	position_ = position;
	x_offset_ = x_offset;
	y_offset_ = y_offset;
}

void Hitbox::SetPosition(glm::vec3 position) {
	position_ = position;
}

void Hitbox::SetOffsets(float in_x, float in_y) {
	x_offset_ = in_x;
	y_offset_ = in_y;
}

float Hitbox::GetXOffset() const {
	return this->x_offset_;
}

glm::vec2 Hitbox::GetPoint0() const
{
	glm::vec2 point;
	point.x = position_.x - x_offset_;
	point.y = position_.y - y_offset_;

	return point;
}

glm::vec2 Hitbox::GetPoint1() const
{
	glm::vec2 point;
	point.x = position_.x + x_offset_;
	point.y = position_.y - y_offset_;

	return point;
}

glm::vec2 Hitbox::GetPoint2() const
{
	glm::vec2 point;
	point.x = position_.x + x_offset_;
	point.y = position_.y + y_offset_;

	return point;
}

glm::vec2 Hitbox::GetPoint3() const
{
	glm::vec2 point;
	point.x = position_.x - x_offset_;
	point.y = position_.y + y_offset_;

	return point;
}

BoxPoints Hitbox::GetPoints() const {
	BoxPoints points;

	points.bottom_left.x = position_.x - x_offset_;
	points.bottom_left.y = position_.y - y_offset_;

	points.bottom_right.x = position_.x + x_offset_;
	points.bottom_right.y = position_.y - y_offset_;

	points.top_right.x = position_.x + x_offset_;
	points.top_right.y = position_.y + y_offset_;

	points.top_left.x = position_.x - x_offset_;
	points.top_left.y = position_.y + y_offset_;

	return points;
}

bool Hitbox::CheckCollision(const BoxPoints& other_box) {
	//Check if any of the points is inside other entity's bounding box
	int x_side = 0;
	int y_side = 0;

	//Check if left side of the other box lies between the vertical
	//sides of this one
	bool left_side = false;
	x_side = other_box.bottom_left.x;

	if (this->GetPoint0().x < x_side	&&	x_side < this->GetPoint1().x) {
		left_side = true;
	}

	//Then do the same check for the right side
	bool right_side = false;
	x_side = other_box.bottom_right.x;

	if (this->GetPoint0().x < x_side	&&	x_side < this->GetPoint1().x) {
		right_side = true;
	}

	//Now check if bottom side of the other box lies between the horizontal
	//sides of this one
	bool bot_side = false;
	y_side = other_box.bottom_left.y;

	if (this->GetPoint0().y < y_side	&&	y_side < this->GetPoint3().y) {
		bot_side = true;
	}

	//Finally check the top side
	bool top_side = false;
	y_side = other_box.top_left.y;

	if (this->GetPoint0().y < y_side	&&	y_side < this->GetPoint3().y) {
		top_side = true;
	}

	//We have a hit if AT LEAST one of the vertical AND one of the
	//horizontal sides are contained, then return true
	if ((left_side || right_side) && (bot_side || top_side)) {
		return true;
	}

	//Otherwise return false
	return false;
}