/*\file VBO.h*/
#include "rendering/VBO.h"

VBO::VBO(int n, float size, void* data) : m_num(n)
{
	//Generate and bind buffer data
	glGenBuffers(m_num, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::~VBO()
{
	//Delete buffers
	glDeleteBuffers(m_num, &m_ID);
}

void VBO::Bind() const
{
	//Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VBO::Unbind() const
{
	//Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


