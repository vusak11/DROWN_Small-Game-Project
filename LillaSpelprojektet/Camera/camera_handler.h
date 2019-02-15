#ifndef CAMERA_CAMERA_HANDLER_H_
#define CAMERA_CAMERA_HANDLER_H_

#include <GLM/glm.hpp>

#include "camera.h"

class CameraHandler {
private:

	enum CameraMode {
		PRIMARY,			//0
		SECONDARY			//1
	};

	Camera cams_[2];		//Primary and secondary camera
	CameraMode mode_;

public:

	CameraHandler(glm::vec3 in_target, float in_dist);
	~CameraHandler();

	void SwapCamera();
	int GetMode();

	glm::mat4 MoveCamera(float in_x, float in_y, float in_z = 0.0f);
	glm::mat4 SetCameraPos(float in_x, float in_y, float in_z = 0.0f);
	glm::mat4 SetPrimaryCameraPos(glm::vec3 p);
	glm::mat4 GetPerspectiveMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetViewPerspectiveMatrix() const;
	glm::vec3 GetCameraPosition() const;

};

#endif // !CAMERA_CAMERA_HANDLER_H
