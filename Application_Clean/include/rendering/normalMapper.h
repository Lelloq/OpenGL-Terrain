/*file normalMapper.h*/
#pragma once
#include <vector>
#include <glm/glm.hpp>

/*\struct vertex
holds vertex data in a structural way so its easier to assign new values to each attribute*/
struct vertex
{
	glm::vec3 pos;
	glm::vec3 normals;
	glm::vec2 uv;
	glm::vec3 tan;
	glm::vec3 bitan;
};

/*\class normalMapper
the class the calculates tan and bitan*/
class normalMapper
{
private:
	std::vector<vertex> m_vertices; //!< vector of vertices stored as a struct
	std::vector<float> m_VertexData;//!< vector of vertices stored as a float once unpacked from the struct
public:
	normalMapper(float* vertices,int length, unsigned int oldstride);//!< constructor with parameters
	void calcTB(unsigned int* indices, int length);//!< void calculate tan and bitan
	void Unpack(unsigned int stride);//!< unpack the vertex into floats
	inline std::vector<float> getNewVertexData() { return m_VertexData; }//!< accessor for m_VertexData
};