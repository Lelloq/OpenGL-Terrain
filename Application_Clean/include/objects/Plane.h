/*\file Plane.h*/
#pragma once
#include "rendering/Renderer.h"
#include "rendering/Texture.h"
#include "rendering/normalMapper.h"

/*\class Plane inherits public Renderer*/
class Plane : public Renderer
{
private:
	VAO* m_VAO;//!< vao object
	VBO* m_VBO;//!< vbo object
	EBO* m_EBO;//!< ebo object
	Texture* m_Dif, *m_Spec, *m_Norm, *m_Parallax;//!< texture object textures

	float planeSize = 20.f; //!< plane size
	float planeLevel = -2.0f;//!< plane level

	float planeVertices[32] = { //!< plane vertices data
	 -planeSize, planeLevel,  -planeSize, 0.0, 1.0, 0.0,0.0f, 0.0f,
	 planeSize, planeLevel,   -planeSize, 0.0, 1.0, 0.0,1.0f, 0.0f,
	 planeSize, planeLevel,  planeSize, 0.0, 1.0, 0.0, 1.0f, 1.0f,
	-planeSize, planeLevel,  planeSize, 0.0, 1.0, 0.0, 0.0f, 1.0f
	};

	unsigned int planeIndices[6] = { //!< plane indices data
		3,2,1,
		3,1,0
	};

public:
	Plane(); //!< default constructor
	~Plane();//!< default destructor

	void Draw() const override; //!< Render object function
	inline Texture* getDiffuse() { return m_Dif; } //!< texture accessor for diffuse texture
	inline Texture* getSpecular() { return m_Spec; } //!< texture accessor for specular texture
	inline Texture* getNormal() { return m_Norm; }//!< texture accessor for normal map
};