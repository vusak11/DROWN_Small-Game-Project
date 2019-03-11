#include "light.h"

Light::Light() {
	position_ = glm::vec3(169.0f, -60.0f, 10.0f);
	brightness_ = glm::vec3(0.7f, 0.7f, 0.7f);
	ambient_light_ = glm::vec3(1.0f, 0.58f, 0.20f);
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
