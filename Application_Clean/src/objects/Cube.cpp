/*\file Cube.cpp*/
#include "objects/Cube.h"

Cube::Cube()
{
	//Get the tan and bitan values for cube vertices
	normalMapper newVerts = normalMapper(m_cubeVertices, 192, 8);
	newVerts.calcTB(m_cubeIndices, 36);
	newVerts.Unpack(14);

	//Create vao vbo ebo diffuse specular and normal objects
	m_VAO = new VAO(1);
	//Uses the new vertex data created by newVerts and puts it into the vbo
	m_VBO = new VBO(1, newVerts.getNewVertexData().size() * sizeof(float), newVerts.getNewVertexData().data());
	m_EBO = new EBO(1, 36, m_cubeIndices);
	m_Dif = new Texture();
	m_Spec = new Texture();
	m_Norm = new Texture();

	//Load textures
	m_Dif->loadTexture("images/metalPlate/diffuse.jpg");
	m_Spec->loadTexture("images/metalPlate/specular.jpg");
	m_Norm->loadTexture("images/metalPlate/normal.jpg");

	//Create layouts
	m_VAO->CreateLayout(*m_VBO, 0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), 0);//Position
	m_VAO->CreateLayout(*m_VBO, 1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));//Normals
	m_VAO->CreateLayout(*m_VBO, 2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));//UV
	m_VAO->CreateLayout(*m_VBO, 3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));//Tan
	m_VAO->CreateLayout(*m_VBO, 4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));//Bitan
}

Cube::~Cube()
{
	//Cleanup
	m_VAO->~VAO();
	m_VBO->~VBO();
	m_EBO->~EBO();
	m_Dif->~Texture();
	m_Spec->~Texture();
	m_Norm->~Texture();
}

void Cube::Draw() const
{
	//Bind then draw elements
	m_VAO->Bind();
	m_Dif->Bind(0);
	m_Spec->Bind(1);
	m_Norm->Bind(2);
	glDrawElements(GL_TRIANGLES, m_EBO->getCount(), GL_UNSIGNED_INT, 0);
}




