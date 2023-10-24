/*\file VAO.h*/
#pragma once
#include"Shader.h"
#include "VBO.h"

/*\class VAO*/
class VAO
{
private:
	GLuint m_ID;//!< Renderer Id
	unsigned int m_num;//!< Number of vbos to make
	static unsigned int boundVAO;
public:
	VAO(int n);//!< Constructor that takes in number of VAOs to make
	~VAO();//!< Destructor
	void Bind();//!< Bind VAO
	void Unbind();//!< Unbind VAO
	void CreateLayout(VBO& vbo, int index, int count, GLenum type, bool normalized, int stride, void* offset);//!< Create layouts for buffer data
};