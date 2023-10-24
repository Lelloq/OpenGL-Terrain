/*\file Texture.h*/
#pragma once
#include <vector>
#include <map>
#include "Shader.h"

/*\class Texture*/
class Texture
{
private:
	unsigned int m_ID; //!< renderer ID
	static std::map<int,int> textureUnits;//!<Static texture unit slot numbers with the texture ID
public:
	Texture(); //!< Default constructor
	~Texture(); //!< Default destructor
	void Bind(unsigned int unit); //!< Bind the texture to the assigned texture unit
	void Unbind(); //!< Unbind the texture
	void loadTexture(const char* filepath);//!< Load texture from the filepath
	void createTexture(unsigned int width, unsigned int height);
	void bindImage(unsigned int unit, int accessMode);//!<0 is write only, 1 is read only, otherwise it's read and write
};