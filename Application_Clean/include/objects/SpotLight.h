/*\file SpotLight.h*/
#pragma once
#include <string>
#include "Lights.h"

/*\struct SpotLightData*/
struct SpotLightData
{
	glm::vec3 colour, position, direction;//!< spotlight colour
	float cutInner, cutOuter;//!< Inner and outer cutoff points
	float kC, lC, qC;//!< attenuation constants
};
/*\class SpotLight*/
class SpotLight : public Lights
{
private:
	std::string m_name;
	SpotLightData m_data;
public:
	SpotLight(std::string name, SpotLightData lightdata);//!<Constructor
	void setLightPosition(Shader* shader) override;//!< inherited function that sets the position uniforms
	void setLightColour(Shader* shader) override;//!< inherited function that sets the colour uniforms
	void setLightDirection(Shader* shader);
	void setLightAttenuation(Shader* shader);//!< set light attenuations
	void setLightCutoffs(Shader* shader);//!< set light inner and outer cutoff points
	inline void changeLightPosition(glm::vec3 position) { m_data.position = position; }//!< change spotlight position
	inline void changeLightDirection(glm::vec3 direction) { m_data.direction = direction; }//!< change spotlight direction
};