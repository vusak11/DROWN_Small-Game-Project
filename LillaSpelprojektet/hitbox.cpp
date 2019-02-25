#include "hitbox.h"
#include "Globals.h"


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

BoxPoints HitBox::GetPoints() const
{
	BoxPoints points;

	points.bottomLeft.x = position_.x - x_offset_;
	points.bottomLeft.y = position_.y - y_offset_;

	points.bottomRight.x = position_.x + x_offset_;
	points.bottomRight.y = position_.y - y_offset_;

	points.TopRight.x = position_.x + x_offset_;
	points.TopRight.y = position_.y + y_offset_;

	points.TopLeft.x = position_.x - x_offset_;
	points.TopLeft.y = position_.y + y_offset_;

	return points;
}


