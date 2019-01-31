#include "light_handler.h"

LightHandler::LightHandler()
{
	//Scrap values
	position_ = glm::vec3(0.0f, 10.0f, 0.0f);
	brightness_ = glm::vec3(0.7f, 0.7f, 0.7f);
	ambient_light_ = glm::vec3(0.2f, 0.2f, 0.2f);
	diffuse_constant_ = glm::vec3(1.0f, 1.0f, 1.0f);
	specular_light_ = glm::vec3(1.0f, 1.0f, 1.0f);
	
}

void LightHandler::LightDefault(glm::vec3 pos, glm::vec3 brightness, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	position_ = pos;
	brightness_ = brightness;
	ambient_light_ = ambient;
	diffuse_constant_ = diffuse;
	specular_light_ = specular;
}

LightHandler::~LightHandler()
{
}

void LightHandler::SetPos(glm::vec3 pos)
{
	position_ = pos;
}

glm::vec3 LightHandler::GetPos() const
{
	return position_;
}

void LightHandler::SetBrightness(glm::vec3 brightness)
{
	brightness_ = brightness;
}

glm::vec3 LightHandler::GetBrightness() const
{
	return brightness_;
}

void LightHandler::SetAmbientLight(glm::vec3 ambient)
{
	ambient_light_ = ambient;
}

glm::vec3 LightHandler::GetAmbientLight() const
{
	return ambient_light_;
}

void LightHandler::SetDiffuseConstant(glm::vec3 diffuse)
{
	diffuse_constant_ = diffuse;
}

glm::vec3 LightHandler::GetDiffuseConstant() const
{
	return diffuse_constant_;
}

void LightHandler::SetSpecularLight(glm::vec3 specular)
{
	specular_light_ = specular;
}

glm::vec3 LightHandler::GetSpecularLight() const
{
	return specular_light_;
}
