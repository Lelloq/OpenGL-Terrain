/*\file PointLight.cpp*/
#include "objects/PointLight.h"
#include <sstream>

PointLight::PointLight(std::string name, PointLightData lightdata) : m_name(name), m_data(lightdata)
{
	//point light data gets assigned the point light data sent through the constructor
	m_data.positions.resize(m_data.size);
	m_data.positions = lightdata.positions;
	m_data.colours.resize(m_data.size);
	m_data.colours = lightdata.colours;
	//Setting attenuation constants
	m_data.kC.resize(m_data.size);
	m_data.kC = lightdata.kC;
	m_data.lC.resize(m_data.size);
	m_data.lC = lightdata.lC;
	m_data.qC.resize(m_data.size);
	m_data.qC = lightdata.qC;
}

//String stream is used to concatonate strings so that the uniform is read properly in each setLightX function
void PointLight::setLightPosition(Shader* shader)
{
	//Set point light position uniforms
	shader->use();
	for(int i = 0; i < m_data.size;i++)
	{
		std::stringstream ss;
		ss << "pointLightTan" << "[" << i << "]";
		shader->setVec3(ss.str(), m_data.positions[i]);
	}
}

void PointLight::setLightColour(Shader* shader)
{
	//Set point light colour uniforms
	shader->use();
	for (int i = 0; i < m_data.size; i++)
	{
		std::stringstream ss;
		ss << m_name << "[" << i << "]" << ".lightColour";
		shader->setVec3(ss.str(), m_data.colours[i]);
	}
}

void PointLight::setLightAttenuation(Shader* shader)
{
	//Set point light attenuation factor uniforms
	shader->use();
	for (int i = 0; i < m_data.size; i++)
	{
		std::stringstream ss;
		ss << m_name << "[" << i << "]" << ".kC";
		shader->setFloat(ss.str(), m_data.kC[i]);
		ss.str("");
		ss << m_name << "[" << i << "]" << ".lC";
		shader->setFloat(ss.str(), m_data.lC[i]);
		ss.str("");
		ss << m_name << "[" << i << "]" << ".qC";
		shader->setFloat(ss.str(), m_data.qC[i]);
	}
}

//Gets the index of the vector and changes its data as long as the index is within range
void PointLight::changeLightPosition(int index, glm::vec3 data)
{
	if(index < m_data.positions.size())
	{
		m_data.positions[index] = data;
	}
}

void PointLight::changeLightColour(int index, glm::vec3 data)
{
	if (index < m_data.colours.size())
	{
		m_data.colours[index] = data;
	}
}