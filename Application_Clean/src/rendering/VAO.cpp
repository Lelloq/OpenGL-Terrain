/*\file VAO.cpp*/
#include "rendering/VAO.h"

unsigned int VAO::boundVAO = 0;
VAO::VAO(int n) : m_num(n)
{
	//Create vertex arrays and bind
	glCreateVertexArrays(n, &m_ID);
	glBindVertexArray(m_ID);
}

VAO::~VAO()
{
	//Delete vertex arrays
	glDeleteVertexArrays(m_num, &m_ID);
}

void VAO::Bind()
{
	//Bind vertex array
	if(boundVAO != m_ID)
	{ 
		boundVAO = m_ID;
		glBindVertexArray(m_ID);
	}
}

void VAO::Unbind()
{
	//Unbind vertex array
	glBindVertexArray(0);
}

void VAO::CreateLayout(VBO& vbo, int index, int count, GLenum type, bool normalized, int stride, void* offset)
{
	//Create the buffer layour
	vbo.Bind();
	glVertexAttribPointer(index, count, type, normalized, stride, offset);
	glEnableVertexAttribArray(index);
	vbo.Unbind();
}
