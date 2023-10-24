/*\file VBO.h*/
#pragma once
#include "Shader.h"

/*\class VBO*/
class VBO
{
private:
	GLuint m_ID;//!< Renderer Id
	unsigned int m_num;//!< Number of vbos to make
public:
	VBO(int n, float size, void* data);//!< Constructor that takes in number of VBOs, vertex data and data
	~VBO();//!< Destructor
	void Bind() const;//!< Bind
	void Unbind() const;//!< Unbind
};