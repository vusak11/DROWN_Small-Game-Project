#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_PURE		//Prevents SIMD functionality, thus nulling a call to include the missing file 'func_trigonometric_simd.inl'.
							//This must be done before the inclusion of 'glm.hpp'
#include <GLM/glm.hpp>

class Camera {
private:
	
	glm::vec3 pos_;		//Camera position
	glm::vec3 look_at_;	//Point camera is looking at
	glm::vec3 dir_;		//Direction from point camera is looking at to camera position
	glm::vec3 up_;		//Direction camera considers upwards
	glm::vec3 right_;	//Direction camera considers rightwards
	
	glm::mat4 view_mat_;			//View matrix
	glm::mat4 perspective_mat_;		//Perspective matrix
	
	void UpdateViewMatrix();
	
public:
	Camera(glm::vec3 in_pos);
	~Camera();

	void MoveCamera(float in_x, float in_y, float in_z = 0.0f);
	//void AngleCamera(float in_x, float in_y, float in_z = 0.0f);
	glm::mat4 GetViewPerspectiveMatrix();

};

#endif