/*\file EBO.cpp*/
#include "rendering/EBO.h"

EBO::EBO(unsigned int n, unsigned int count, unsigned int* data) : m_num(n), m_count(count)
{
	//Generate and bind ebo with the data and size
	glGenBuffers(m_num, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

EBO::~EBO()
{
	//Delete ebo
	glDeleteBuffers(m_num, &m_ID);
}

void EBO::Bind() const
{
	//Bind ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void EBO::Unbind() const
{
	//Unbind ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


