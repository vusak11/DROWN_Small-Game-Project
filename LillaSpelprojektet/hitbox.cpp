#include "hitbox.h"



HitBox::HitBox() {
	position_ = glm::vec3(1.0f, 1.0f, 1.0f);
	x_offset_ = 1;
	y_offset_ = 1;
}

HitBox::HitBox(glm::vec3 position, float x_offset, float y_offset) {
	position_ = position;
	x_offset_ = x_offset;
	y_offset_ = y_offset;
}

HitBox::~HitBox() {
}

void HitBox::UpdateHitbox(glm::vec3 position, float x_offset, float y_offset) {
	position_ = position;
	x_offset_ = x_offset;
	y_offset_ = y_offset;
}

glm::vec2 HitBox::GetPoint0() const
{
	glm::vec2 point;
	point.x = position_.x - x_offset_;
	point.y = position_.y - y_offset_;

	return point;
}

glm::vec2 HitBox::GetPoint1() const
{
	glm::vec2 point;
	point.x = position_.x + x_offset_;
	point.y = position_.y - y_offset_;

	return point;
}

glm::vec2 HitBox::GetPoint2() const
{
	glm::vec2 point;
	point.x = position_.x + x_offset_;
	point.y = position_.y + y_offset_;

	return point;
}

glm::vec2 HitBox::GetPoint3() const
{
	glm::vec2 point;
	point.x = position_.x - x_offset_;
	point.y = position_.y + y_offset_;

	return point;
}

glm::vec2* HitBox::GetPoints() const
{
	glm::vec2 points[4];
	points[0].x = position_.x - x_offset_;
	points[0].y = position_.y - y_offset_;
	
	points[1].x = position_.x + x_offset_;
	points[1].y = position_.y - y_offset_;

	points[2].x = position_.x + x_offset_;
	points[2].y = position_.y + y_offset_;

	points[3].x = position_.x - x_offset_;
	points[3].y = position_.y + y_offset_;

	return points;
}
