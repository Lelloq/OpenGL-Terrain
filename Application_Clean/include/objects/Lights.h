/*\file Lights.h*/
#pragma once
#include <glm/glm.hpp>
#include "rendering/Shader.h"

/*\class Lights
Abstract lighting class*/
class Lights
{
public:
	~Lights() = default;
	virtual void setLightPosition(Shader* shader) = 0;
	virtual void setLightColour(Shader* shader) = 0;
};
