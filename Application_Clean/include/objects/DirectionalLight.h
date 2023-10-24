/*\file DirectionalLight*/
#pragma once
#include "Lights.h"

/*\class DirectionalLight*/
class DirectionalLight : public Lights
{
private:
	std::string m_name;//!< uniform name
	glm::vec3 m_position, m_colour;//!< 3 floats of position and colour
public:
	DirectionalLight(std::string name, glm::vec3 position, glm::vec3 colour);//!< Constructor that takes in uniform name and 2 vec3 floats
	~DirectionalLight() {};
	void setLightPosition(Shader* shader) override;//!< set light position uniform
	void setLightColour(Shader* shader) override;//!< set light colour uniform
	inline void changeLightPos(glm::vec3 position) { m_position = position; }; //!< set light position
	inline void changeLightColour(glm::vec3 colour) { m_colour = colour; };//!< set light colour
};
