#include "camera_handler.h"

CameraHandler::CameraHandler(glm::vec3 in_target, float in_dist) {
	this->cams_[PRIMARY] = Camera(in_target, in_dist);
	this->cams_[SECONDARY] = Camera(in_target, GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_ZOOM"));			//Place the secondary camera (debug) a bit further away than the primary one
	this->mode_ = PRIMARY;
}

CameraHandler::~CameraHandler() {

}

void CameraHandler::SwapCamera() {
	//Swap to the other camera
	if (this->mode_ == PRIMARY) { this->mode_ = SECONDARY; }
	else { this->mode_ = PRIMARY; }
}

void CameraHandler::SwapCameraToBossCamera() {
	this->mode_ = BOSS;
	SetCameraPos(160.0f, -1152.0f, 100.0f);
}

int CameraHandler::GetMode() {
	return this->mode_;
}

glm::mat4 CameraHandler::MoveCamera(float in_x, float in_y, float in_z) {
	//Update the active camera's position
	this->cams_[this->mode_].MoveCamera(in_x, in_y, in_z);

	//Return the active camera's View*Perspective Matrix
	return this->cams_[this->mode_].GetViewPerspectiveMatrix();
}

glm::mat4 CameraHandler::SetCameraPos(float in_x, float in_y, float in_z) {
	//Update the active camera's position
	this->cams_[this->mode_].SetCameraPos(in_x, in_y, in_z);

	//Return the active camera's View*Perspective Matrix
	return this->cams_[this->mode_].GetViewPerspectiveMatrix();
}

glm::mat4 CameraHandler::SetPrimaryCameraPos(glm::vec3 position)
{
	this->cams_[PRIMARY].SetCameraPos(position.x, position.y, cams_[0].GetCameraPosition().z);

	//Return the active camera's View*Perspective Matrix
	return this->cams_[PRIMARY].GetViewPerspectiveMatrix();
}

glm::mat4 CameraHandler::GetPerspectiveMatrix() const
{
	return cams_[mode_].GetPerspectiveMatrix();
}

glm::mat4 CameraHandler::GetViewMatrix() const
{
	return cams_[mode_].GetViewMatrix();
}

glm::mat4 CameraHandler::GetViewPerspectiveMatrix() const {
	return cams_[mode_].GetViewPerspectiveMatrix();
}

glm::vec3 CameraHandler::GetCameraPosition() const
{
	return cams_[mode_].GetCameraPosition();
}