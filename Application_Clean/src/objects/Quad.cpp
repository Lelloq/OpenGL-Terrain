/*\file Quad.cpp*/
#include "objects/Quad.h"

Quad::Quad()
{
	//Create vao vbo ebo diffuse specular and normal objects
	m_VAO = new VAO(1);
	//Uses the new vertex data created by newVerts and puts it into the vbo
	m_VBO = new VBO(1, sizeof(quadVerts), quadVerts);
	m_EBO = new EBO(1, 6, quadIndices);

	m_VAO->CreateLayout(*m_VBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	m_VAO->CreateLayout(*m_VBO, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

Quad::~Quad()
{
	//Cleanup
	m_VAO->~VAO();
	m_VBO->~VBO();
	m_EBO->~EBO();
}

void Quad::setAttachment(unsigned int ID, unsigned int unit)
{
	//Bind texture to a slot
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Quad::Draw() const
{
	m_VAO->Bind();
	glDrawElements(GL_TRIANGLES, m_EBO->getCount(), GL_UNSIGNED_INT, 0);
}
