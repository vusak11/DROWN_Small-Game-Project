#include "hitbox.h"
//#include <iostream>	//For debug



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
	
	//
	//	y3	---------			ts	---------
	//		|		|				|		| 
	//		|  Box  |				|  Box  |
	//		|		|				|		| 
	//	y0	---------			bs	---------
	//		x0     x1				ls     rs
	//
	// Each of the denotations of the side represents a
	// line on the x-y-plane. We know the boxes to *not*
	// overlap if the top line of one box is lower than
	// the bottom line of the other *or* the rightmost
	// line of one box is further leftwards than the
	// leftmost of the other

	//We start by assuming that the boxes are overlapping
	bool ls_x1 = true;	//ls < x0
	bool x0_rs = true;	//x0 < rs
	bool bs_y3 = true;	//bs < y3
	bool y0_ts = true;	//y0 < ts

	//Check if any of the sides breaks the criteria
	//(note the >)
	int side = other_box.bottom_left.x;
	if (side > this->GetPoint1().x) { ls_x1 = false; }

	side = other_box.bottom_right.x;
	if (this->GetPoint0().x > side) { x0_rs = false; }

	side = other_box.bottom_left.y;
	if (side > this->GetPoint3().y) { bs_y3 = false; }

	side = other_box.top_left.y;
	if (this->GetPoint0().y > side) { y0_ts = false; }

	//DEBUG
	//std::cout << "		Result:"
	//	<< "\n		ls_x1 : " << ls_x1
	//	<< "\n		x0_rs : " << x0_rs
	//	<< "\n		bs_y3 : " << bs_y3
	//	<< "\n		y0_ts : " << y0_ts
	//	<< "\n		Final : " << (ls_x1 && x0_rs && bs_y3 && y0_ts)
	//	<< std::endl;
	//DEBUG

	//We need the all the assumptions to be true
	//If one has been proven false we do not have a overlap
	if (ls_x1 && x0_rs && bs_y3 && y0_ts) {
		return true;
	}

	//Otherwise return false
	return false;
}