/*\file EBO.h*/
#pragma once
#include "Shader.h"

/*\class EBO*/
class EBO
{
private:
	GLuint m_ID; //!< Renderer ID
	unsigned int m_num; //!< Number of index buffers
	unsigned int m_count;//!< Indices count
public:
	EBO(unsigned int n, unsigned int count, unsigned int* data); //!< Constructor that takes in number of EBOs, indices data and data
	~EBO();//!< Destructor
	void Bind() const;//!< Bind ebo
	void Unbind() const;//!< unbind ebo

	inline unsigned int getCount() { return m_count; }//!< get the indices count
};