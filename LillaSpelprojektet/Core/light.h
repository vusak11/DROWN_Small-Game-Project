#ifndef LIGHT_H
#define LIGHT_H

#include"GLM/glm.hpp"

class Light {
private:
	glm::vec3 position_;
	glm::vec3 brightness_;
	glm::vec3 ambient_light_;
public:
	Light();
	~Light();

	void SetPos(glm::vec3 pos);
	glm::vec3 GetPos() const;

	void SetBrightness(glm::vec3 brightness);
	glm::vec3 GetBrightness() const;

	void SetAmbientLight(glm::vec3 ambient);
	glm::vec3 GetAmbientLight() const;
};

#endif