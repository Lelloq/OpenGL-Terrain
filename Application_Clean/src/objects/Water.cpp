/*\file Water.cpp*/
#include "objects/Water.h"
#include <iostream>

//Creating the grid the same way as terrain object
void Water::makeVertex(float x, float z)
{
	//x y z position
	m_vertices.push_back((float)x); //xPos
	m_vertices.push_back(m_waterLevel); //yPos - always 0 for now. Going to calculate this on GPU - could change to calclaute it here.
	m_vertices.push_back((float)z); //zPos

	// add texture coords
	m_vertices.push_back((float)x / (m_waterSize * m_cellSize));
	m_vertices.push_back((float)z / (m_waterSize * m_cellSize));

	m_vertexCount++;
}

void Water::makeIndices()
{
	int max_index = (m_vertexCount / 4) * 6;
	m_indices.resize(max_index);
	int offset = 0;
	for (int i = 0; i < m_indices.size(); i += 6)
	{
		m_indices.at(i + 0) = 0 + offset;
		m_indices.at(i + 1) = 1 + offset;
		m_indices.at(i + 2) = 2 + offset;

		m_indices.at(i + 3) = 2 + offset;
		m_indices.at(i + 4) = 3 + offset;
		m_indices.at(i + 5) = 0 + offset;

		offset += 4;
	}
}

Water::Water(float size, float level)
{
	m_waterSize = size;
	m_waterLevel = level;

	for (int z = 0; z < m_waterSize - 1; z++) {
		float  offSetZ = z * m_cellSize;
		for (int x = 0; x < m_waterSize - 1; x++) {
			float offSetX = x * m_cellSize;
			makeVertex(offSetX, offSetZ);  // a
			makeVertex(offSetX, offSetZ + m_cellSize);  //e
			makeVertex(offSetX + m_cellSize, offSetZ + m_cellSize);  //f
			makeVertex(offSetX + m_cellSize, offSetZ);   // c
		}
	}
	
	makeIndices();

	//Create VAO VBO EBO
	m_VAO = new VAO(1);
	m_VBO = new VBO(1, m_vertices.size() * sizeof(float), m_vertices.data());
	m_EBO = new EBO(1, m_indices.size(), m_indices.data());

	m_VAO->CreateLayout(*m_VBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);//Position
	m_VAO->CreateLayout(*m_VBO, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));//UV

	//Create Textures
	m_normals = new Texture();
	m_normals->loadTexture("../Images/water/waterNormals.png");
	m_dudv = new Texture();
	m_dudv->loadTexture("../Images/water/waterDUDV.png");

	//Create FBOs
	m_reflection = new FBO(1, false);
	m_refraction = new FBO(1, false);
	m_depth = new DepthMap();
}

Water::~Water()
{
	m_VAO->~VAO();
	m_VBO->~VBO();
	m_EBO->~EBO();
	m_reflection->~FBO();
	m_refraction->~FBO();
	m_depth->~DepthMap();
	m_dudv->~Texture();
	m_normals->~Texture();
}

void Water::Draw() const
{
	m_VAO->Bind();
	m_VBO->Bind();
	m_EBO->Bind();
	glDrawElements(GL_TRIANGLES, m_EBO->getCount(), GL_UNSIGNED_INT, 0);
}

void Water::bindReflection() const
{
	m_reflection->bind();
}

void Water::bindRefraction() const
{
	m_refraction->bind();
}

void Water::unbind() const
{
	m_reflection->unbind();
	m_refraction->unbind();
}

void Water::enableClipPlane()
{
	glEnable(GL_CLIP_DISTANCE0);
}

void Water::disableClipPlane()
{
	glDisable(GL_CLIP_DISTANCE0);
}

void Water::setAttachments(int reflec, int reflecAttachmentIndex, int refrac, int refracAttachmentIndex)
{
	glActiveTexture(GL_TEXTURE0 + reflec);
	glBindTexture(GL_TEXTURE_2D, m_reflection->getID(reflecAttachmentIndex));
	
	glActiveTexture(GL_TEXTURE0 + refrac);
	glBindTexture(GL_TEXTURE_2D, m_refraction->getID(refracAttachmentIndex));
}

void Water::bindWaterTextures(int dudv, int normals)
{
	m_dudv->Bind(dudv);
	m_normals->Bind(normals);
}

void Water::setDepthAttachment(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_depth->getID());
}

void Water::bindDepth() const
{
	m_depth->bind();
}

void Water::unbindDepth() const
{
	m_depth->unbind();
}


