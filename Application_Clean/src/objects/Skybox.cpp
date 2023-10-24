/*\file Skybox.cpp*/
#include "objects/Skybox.h"
#include <iostream>
#include <stb_image.h>

Skybox::Skybox()
{
	//Generate textures, vao and vbo
	glGenTextures(1, &m_ID);
	m_VAO = new VAO(1);
	m_VBO = new VBO(1, sizeof(m_skyboxVertices), m_skyboxVertices);
	m_VAO->CreateLayout(*m_VBO, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
}

Skybox::~Skybox()
{
	//Delete skybox texture
	glDeleteTextures(1, &m_ID);
}

void Skybox::Bind(unsigned int unit)
{
	//Bind texture to a slot
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void Skybox::Unbind()
{
	//Unbind texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::Draw()
{
	//Removes depth mask temporarily to prevent other objects being drawn under the skybox
	glDepthMask(GL_FALSE);
	m_VAO->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void Skybox::loadCubemap(std::vector<std::string> faces)
{
	//Load each image sent by the load cubemap function
	int width, height, channels;
	for (int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, NULL);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture at: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	//Texture parameter settings
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
