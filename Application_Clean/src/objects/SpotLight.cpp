/*\file SpotLight.cpp*/
#include "objects/SpotLight.h"

SpotLight::SpotLight(std::string name, SpotLightData lightdata)
{
	//Assings the data to the class attributes based on the given spotlight data in the given parameter
	m_name = name;

	m_data.colour = lightdata.colour;
	m_data.cutInner = lightdata.cutInner;
	m_data.cutOuter = lightdata.cutOuter;

	m_data.kC = lightdata.kC;
	m_data.lC = lightdata.lC;
	m_data.qC = lightdata.qC;

	m_data.position = {0,0,0};
	m_data.colour = lightdata.colour;
	m_data.direction = {0,0,0};
}

void SpotLight::setLightPosition(Shader* shader)
{
	//Sets spotlight position uniform
	shader->use();
	shader->setVec3("spotLightTan", m_data.position);
}

void SpotLight::setLightColour(Shader* shader)
{
	//Sets spotlight colour uniform
	shader->use();
	shader->setVec3(m_name + ".lightColour", m_data.colour);
}

void SpotLight::setLightDirection(Shader* shader)
{
	//Sets spotlight direction uniform
	shader->use();
	shader->setVec3("spotLightTanDir", m_data.direction);
}

void SpotLight::setLightAttenuation(Shader* shader)
{
	//Sets spotlight attenuation factors uniform
	shader->use();
	shader->setFloat(m_name + ".kC", m_data.kC);
	shader->setFloat(m_name + ".lC", m_data.lC);
	shader->setFloat(m_name + ".qC", m_data.qC);
}

void SpotLight::setLightCutoffs(Shader* shader)
{
	//Sets spotlight cutoff angles uniform
	shader->use();
	shader->setFloat(m_name + ".cutinner", glm::cos(glm::radians(m_data.cutInner)));
	shader->setFloat(m_name + ".cutouter", glm::cos(glm::radians(m_data.cutOuter)));
}