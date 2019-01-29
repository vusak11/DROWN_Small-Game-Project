#include "camera_handler.h"

CameraHandler::CameraHandler(glm::vec3 in_target, float in_dist) {
	this->cams_[PRIMARY] = Camera(in_target, in_dist);
	this->cams_[SECONDARY] = Camera(in_target, in_dist+30);			//Place the secondary camera (debug) a bit further away than the primary one
	this->mode_ = PRIMARY;
}

CameraHandler::~CameraHandler() {

}

void CameraHandler::SwapCamera() {
	//Swap to the other camera
	if (this->mode_ == PRIMARY) { this->mode_ = SECONDARY; }
	else { this->mode_ = PRIMARY; }
}

glm::mat4 CameraHandler::UpdateCamera(float in_x, float in_y, float in_z) {
	//Update the active camera's position
	this->cams_[this->mode_].MoveCamera(in_x, in_y, in_z);

	//Return the active camera's View*Perspective Matrix
	return this->cams_[this->mode_].GetViewPerspectiveMatrix();
}