/*\file Quad.h*/
#pragma once
#include "rendering/renderer.h"
#include "rendering/Texture.h"

/*\class Quad
a 2D rectangle*/
class Quad : public Renderer
{
private:
	VAO* m_VAO;//!< vao object
	VBO* m_VBO;//!< vbo object
	EBO* m_EBO;//!< ebo object

	float quadVerts[20] =
	{
		-1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	
	unsigned int quadIndices[6] =
	{
		3,2,1,
		3,1,0
	};
public:
	Quad();//!<Constructor
	~Quad();//!<Destructor

	void setAttachment(unsigned int ID, unsigned int unit);//!<Bind the texture to a slot
	void Draw() const override;//!<Draw quad
};