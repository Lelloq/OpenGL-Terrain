/*****************************************************************//**
 * \file   Water.h
 * \brief  water rendering
 * 
 * \date   April 2023
 *********************************************************************/
#pragma once
#include "rendering/Renderer.h"
#include "rendering/Texture.h"
#include "rendering/FBO.h"
#include <vector>

class Water : public Renderer
{
private:
	float m_waterSize = 100.f; //!< water size
	float m_waterLevel = -2.0f;//!< water level
	int m_cellSize = 10;
	int m_vertexCount = 0;

	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;

	VAO* m_VAO;
	VBO* m_VBO;
	EBO* m_EBO;
	FBO* m_reflection;
	FBO* m_refraction;
	DepthMap* m_depth;
	Texture* m_dudv;
	Texture* m_normals;

	void makeVertex(float x, float z);
	void makeIndices();
public:
	Water(float size, float level);
	~Water();
	void Draw() const override;
	//Colour
	void bindReflection() const;
	void bindRefraction() const;
	void unbind() const;
	void enableClipPlane();
	void disableClipPlane();
	void setAttachments(int reflec, int reflecLevel, int refrac, int refracLevel);
	void bindWaterTextures(int dudv, int normals);

	//Depth
	void setDepthAttachment(int slot);
	void bindDepth() const;
	void unbindDepth() const;
};
