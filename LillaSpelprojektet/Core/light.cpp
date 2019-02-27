#include "light.h"

Light::Light() {
	//Scrap values
	position_ = glm::vec3(169.0f, -60.0f, 20.0f);
	brightness_ = glm::vec3(0.7f, 0.7f, 0.7f);
	ambient_light_ = glm::vec3(1.0f, 0.6f, 0.0f);
	diffuse_constant_ = glm::vec3(1.0f, 1.0f, 1.0f);
	specular_light_ = glm::vec3(1.0f, 0.1f, 0.1f);
}

void Light::LightDefault(glm::vec3 pos, glm::vec3 brightness, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	position_ = pos;
	brightness_ = brightness;
	ambient_light_ = ambient;
	diffuse_constant_ = diffuse;
	specular_light_ = specular;
}

Light::~Light() {

}

void Light::SetPos(glm::vec3 pos) {
	position_ = pos;
}

glm::vec3 Light::GetPos() const {
	return position_;
}

void Light::SetBrightness(glm::vec3 brightness) {
	brightness_ = brightness;
}

glm::vec3 Light::GetBrightness() const {
	return brightness_;
}

void Light::SetAmbientLight(glm::vec3 ambient) {
	ambient_light_ = ambient;
}

glm::vec3 Light::GetAmbientLight() const {
	return ambient_light_;
}

void Light::SetDiffuseConstant(glm::vec3 diffuse) {
	diffuse_constant_ = diffuse;
}

glm::vec3 Light::GetDiffuseConstant() const {
	return diffuse_constant_;
}

void Light::SetSpecularLight(glm::vec3 specular) {
	specular_light_ = specular;
}

glm::vec3 Light::GetSpecularLight() const {
	return specular_light_;
}
