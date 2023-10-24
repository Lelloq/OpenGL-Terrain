/*\file normalMapper.cpp*/
#include "rendering/normalMapper.h"

//Constructor that sets up the vertex data with empty tan and bitan to be calculated later
normalMapper::normalMapper(float* vertices, int length, unsigned int oldstride)
{
	vertex v;
	for(int i = 0; i < length; i += oldstride)
	{
		v.pos = glm::vec3(vertices[i], vertices[i+1], vertices[i+2]);
		v.normals = glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]);
		v.uv = glm::vec2(vertices[i+6], vertices[i+7]);
		v.tan = glm::vec3(0,0,0);
		v.bitan = glm::vec3(0,0,0);
		m_vertices.push_back(v);
	}
}

//Calculate tan and bitangent
void normalMapper::calcTB(unsigned int* indices, int length)
{
	//Get the vertex from the indices and find the edge and calculate the tan and bitan using a formula
	for(int i = 0; i < length; i += 3)
	{
		vertex& a = m_vertices.at(indices[i]);
		vertex& b = m_vertices.at(indices[i+1]);
		vertex& c = m_vertices.at(indices[i+2]);

		glm::vec3 dPos1 = b.pos - a.pos;
		glm::vec3 dPos2 = c.pos - a.pos;

		glm::vec2 dUv1 = b.uv - a.uv;
		glm::vec2 dUv2 = c.uv - a.uv;

		float r = 1.0f / (dUv1.x * dUv2.y - dUv1.y * dUv2.x);

		glm::vec3 tan = (dPos1 * dUv2.y - dPos2 * dUv1.y) * r;
		glm::vec3 bitan = (dPos2 * dUv1.x - dPos1 * dUv2.x) * r;

		a.tan = a.tan + tan;
		a.bitan = a.bitan + bitan;

		b.tan = b.tan + tan;
		b.bitan = b.bitan + bitan;

		c.tan = c.tan + tan;
		c.bitan = c.bitan + bitan;

	}
	//Normalize the vertices
	for(vertex& v : m_vertices)
	{
		v.tan = glm::normalize(v.tan);
		v.bitan = glm::normalize(v.bitan);
	}
}

//Put the vertex struct into a float so that it can be passed into the VBO
void normalMapper::Unpack(unsigned int stride)
{
	for(vertex v : m_vertices)
	{
		float data[] =
		{
			v.pos.x, v.pos.y,v.pos.z,
			v.normals.x, v.normals.y,v.normals.z,
			v.uv.x,v.uv.y,
			v.tan.x, v.tan.y,v.tan.z,
			v.bitan.x, v.bitan.y,v.bitan.z
		};

		for(int i = 0; i < stride; i++)
		{
			m_VertexData.push_back(data[i]);
		}
	}
}