#ifndef CAMERA_CAMERA_HANDLER_H_
#define CAMERA_CAMERA_HANDLER_H_

#include <GLM/glm.hpp>

#include "camera.h"

class CameraHandler {
private:

	enum CamMode {
		PRIMARY,			//0
		SECONDARY			//1
	};

	Camera cams_[2];		//Primary and secondary camera
	CamMode mode_;

public:

	CameraHandler(glm::vec3 in_target, float in_dist);
	~CameraHandler();

	void SwapCamera();

	glm::mat4 MoveCamera(float in_x, float in_y, float in_z = 0.0f);
	glm::mat4 SetCameraPos(float in_x, float in_y, float in_z = 0.0f);

};

#endif // !CAMERA_CAMERA_HANDLER_H
