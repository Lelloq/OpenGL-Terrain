/*\file PointLight.h*/
#pragma once
#include <string>
#include <vector>
#include "Lights.h"

/*\struct PointLightData*/
struct PointLightData
{
	int size;//!<How many lights there are
	std::vector<float> kC, lC, qC;//!<Attenuation factors
	std::vector<glm::vec3> positions, colours;//!<Light position and colour

	//Resizes the vectors
	void resize()
	{
		kC.resize(size);
		lC.resize(size);
		qC.resize(size);
		positions.resize(size);
		colours.resize(size);
	}
	//Fill vectors with data
	void addConstants(std::vector<float> values)//!< Add constant attenuation values
	{
		for(int i = 0; i < values.size(); i++)
		{
			kC[i] = values[i];
		}
	} 
	void addLinears(std::vector<float> values) //!< Add linear attenuation values
	{
		for(int i = 0; i < values.size(); i++)
		{
			lC[i] = values[i];
		}
	} 
	void addQuadratics(std::vector<float> values) //!< Add quadratic attenuation values
	{
		for(int i = 0; i < values.size(); i++)
		{
			qC[i] = values[i];
		}
	} 
	void addPositions(std::vector<glm::vec3> values)//!< Add light positions
	{
		for (int i = 0; i < values.size(); i++)
		{
			positions[i] = values[i];
		}
	} 
	void addColours(std::vector<glm::vec3> values)//!< Add light colours
	{
		for (int i = 0; i < values.size(); i++)
		{
			colours[i] = values[i];
		}
	} 
};

/*\class PointLight*/
class PointLight : public Lights
{
private:
	std::string m_name;//!< uniform name
	PointLightData m_data;//!< point light data
public:
	PointLight(std::string name, PointLightData lightdata);//!< Constructor that takes in the uniform name and light data
	~PointLight() {};
	void setLightPosition(Shader* shader) override;//!< inherited function that sets the position uniforms
	void setLightColour(Shader* shader) override;//!< inherited function that sets the colour uniforms
	void setLightAttenuation(Shader* shader);//!< function that sets the light attenuation uniforms
	void changeLightPosition(int index, glm::vec3 data);//!< change a specific light's position
	void changeLightColour(int index, glm::vec3 data);//!< change a specific light's colour
};