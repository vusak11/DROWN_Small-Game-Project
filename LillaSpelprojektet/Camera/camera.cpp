#include "Camera.h"

//Private:

void Camera::UpdateViewMatrix() {
	this->view_mat_ = glm::lookAt(this->pos_, this->pos_ + this->dir_, this->up_);
}


//Public:

Camera::Camera(glm::vec3 in_look_at, float in_dist) {
	
	//Point to look at (for example the position of the character)
	this->look_at_ = in_look_at;

	//Set position of camera to be offset from the point of interest (offset on z-axis)
	this->pos_ = in_look_at;
	this->pos_.z += in_dist;
	

	//Create vector with direction to look_at point ( always [0,0,-1] )
	//this->camera_arr_[0].dir = glm::normalize(this->camera_arr_[0].pos - this->camera_arr_[0].look_at);
	this->dir_ = glm::vec3(0.0f, 0.0f, -1.0f);

	//Set up and right vectors
	this->up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right_ = glm::normalize(glm::cross(this->up_, this->dir_));

	//Calculate view matrix
	this->UpdateViewMatrix();

	//Calculate perspective matrix
	float fov = glm::radians(45.0f);
	float aspect = ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH);
	float nearplane = 0.1f;
	float farplane = 1000;
	this->perspective_mat_ = glm::perspective(fov, aspect, nearplane, farplane);
}

Camera::~Camera() {

}

void Camera::MoveCamera(float in_x, float in_y, float in_z) {
	//Update position of camera
	this->pos_.x += in_x;
	this->pos_.y += in_y;
	this->pos_.z += in_z;

	//Update position we look at (to make camera statically follow a point)
	this->look_at_.x += in_x;
	this->look_at_.y += in_y;
	this->look_at_.z += in_z;

	this->UpdateViewMatrix();
}

glm::mat4 Camera::GetViewPerspectiveMatrix() {
	return this->view_mat_ * this->perspective_mat_;
}