#pragma once
#include"GLM/glm.hpp"

class Light {
private:
	glm::vec3 position_;
	glm::vec3 brightness_;
	glm::vec3 diffuse_constant_;
	glm::vec3 specular_light_;
	glm::vec3 ambient_light_;
public:
	Light();
	void LightDefault(glm::vec3 pos, glm::vec3 brightness, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~Light();

	void SetPos(glm::vec3 pos);
	glm::vec3 GetPos() const;

	void SetBrightness(glm::vec3 brightness);
	glm::vec3 GetBrightness() const;

	void SetAmbientLight(glm::vec3 ambient);
	glm::vec3 GetAmbientLight() const;

	void SetDiffuseConstant(glm::vec3 diffuse);
	glm::vec3 GetDiffuseConstant() const;

	void SetSpecularLight(glm::vec3 specular);
	glm::vec3 GetSpecularLight() const;
};