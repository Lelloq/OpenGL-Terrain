#include "objects/Terrain.h"
#include <glad/glad.h>

// constructors

Terrain::Terrain()
{
	makeVertices();
	makeIndices();
	setVAO();

	m_heightMap = new Texture();
	m_heightMap->loadTexture("../Images/terrain/heightMap.jpg");
	m_heightMap->Bind(0);
}

Terrain::~Terrain()
{
	m_VAO->~VAO();
	m_EBO->~EBO();
	m_VBO->~VBO();
}

Terrain::Terrain(int widthIn, int heightIn, int stepSizeIn) :
   m_cellSize(stepSizeIn),
   m_width(widthIn),
   m_height(heightIn)
{
	makeVertices();
	makeIndices();
	setVAO();
}


// render function, binds VAO and makes draw call
// you will certainly want to update this - for example with a renderer class and pass data, material, and model matrix to renderer
void Terrain::render()
{
	Renderer::DrawPatch(*m_VAO, *m_EBO);
}

// creates VAO and VBO from vertex data
// initially only two layouts - one for position and one for UVs
void Terrain::setVAO()
{
	m_VAO = new VAO(1);
	m_VBO = new VBO(1, m_vertices.size() * sizeof(float), m_vertices.data());
	m_EBO = new EBO(1, m_indices.size(), m_indices.data());

	m_VAO->CreateLayout(*m_VBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	m_VAO->CreateLayout(*m_VBO, 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

int Terrain::getSize()
{
	return m_vertices.size();
}


void Terrain::makeVertices()
{
	/* triangle a b c
		   b
		   | \
		   a _ c


		 triangle d f e
		   f _ e
			 \ |
			   d

		 c == d
		 b == f
		 Duplicate vertices, you should add indexing

		a = (x,y,z)
		b = (x, y+1)
		c = (x+1,y)

		d = (x+1,y)
		e = (x, y+1)
		f = (x+1,y+1)

		 each vertex a, b,c, etc. will have 5 data:
		 x y z u v
		  */

	for (int z = 0; z < m_height - 1; z++) {
		float  offSetZ = z * m_cellSize;
		for (int x = 0; x < m_width - 1; x++) {
			float offSetX = x * m_cellSize;
			makeVertex(offSetX, offSetZ);  // a
			makeVertex(offSetX, offSetZ + m_cellSize);  //e
			makeVertex(offSetX + m_cellSize, offSetZ + m_cellSize);  //f
			makeVertex(offSetX + m_cellSize, offSetZ);   // c
		}
	}
}

void Terrain::makeVertex(int x, int z)
{
		//x y z position
	m_vertices.push_back((float)x); //xPos
	m_vertices.push_back(0.0f); //yPos - always 0 for now. Going to calculate this on GPU - could change to calclaute it here.
	m_vertices.push_back((float)z); //zPos

	   // add texture coords
	m_vertices.push_back((float)x / (m_width * m_cellSize));
	m_vertices.push_back((float)z / (m_height * m_cellSize));

	m_vertexCount++;
}

void Terrain::makeIndices()
{
	int max_index = (m_vertexCount / 4) * 6;
	m_indices.resize(max_index);
	int offset = 0;
	for(int i = 0; i < m_indices.size(); i += 6)
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



