/*\file DirectionalLight.cpp*/
#include "objects/DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string name, glm::vec3 position, glm::vec3 colour) : m_name(name), m_position(position), m_colour(colour) {}

void DirectionalLight::setLightPosition(Shader* shader)
{
	//Set directional light uniform
	shader->use();
	shader->setVec3("dirLightTan", m_position);
}

void DirectionalLight::setLightColour(Shader* shader)
{
	//Set directional light colour uniform
	shader->use();
	shader->setVec3(m_name+".lightColour", m_colour);
}
