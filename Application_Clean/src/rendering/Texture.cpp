#include "rendering/Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<int, int> Texture::textureUnits;
//Generate texture id
Texture::Texture()
{
	glGenTextures(1, &m_ID);
}

//Delete generated texture id
Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

//Bind the texture to the active texture unit slot
void Texture::Bind(unsigned int unit)
{
	//If the texture unit isn't bound already then bind it
	if(textureUnits[GL_TEXTURE0 + unit] != m_ID)
	{
		textureUnits[GL_TEXTURE0 + unit] = m_ID;
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}

//Unbind the texture
void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadTexture(const char* filepath)
{
	//Store data into the unsigned char when loading the image file using stb_image
	int width, height, channels;
	unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);

	//If there's data
	if(data)
	{
		//Choose the right format based on the data gathered above
		GLenum format;
		switch(channels)
		{
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		default: format = GL_RGB;
		}

		//Set active texture to 0 and bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		//Create the texture image with the parameters
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		//Texture parameter settings
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Free data from memory so no memory leaks
		stbi_image_free(data);
	}
	else//Output if there is no data
	{
		std::cout << "Failed to load texture at: " << filepath << std::endl;
		stbi_image_free(data);
	}
}

void Texture::createTexture(unsigned int width, unsigned int height)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTextureStorage2D(m_ID, 1, GL_RGBA32F, width, height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::bindImage(unsigned int unit, int accessMode)
{
	glBindImageTexture(unit, m_ID, 0, GL_FALSE, 0, accessMode, GL_RGBA32F);
}
