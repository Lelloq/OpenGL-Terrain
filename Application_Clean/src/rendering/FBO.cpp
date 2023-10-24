/*\file FBO.cpp*/
#include "rendering/FBO.h"
#include "Properties.h"
#include <iostream>

FBO::FBO(GLuint numOfAttachments, bool hdr)
{
	m_attachmentID.resize(numOfAttachments);
	std::vector<GLuint> attachments;

	//Generate and bind framebuffer
	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glGenTextures(numOfAttachments, m_attachmentID.data());//Generate number of textures for multitarget rendering

	//Bind textures to the framebuffer
	for(int i = 0; i < numOfAttachments; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_attachmentID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if(hdr)glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		else   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_attachmentID[i], 0);

		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	//Create rbo for depth rendering
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR: Framebuffer incomplete!" << std::endl;
	}

	//Draw the frame textures
	glDrawBuffers(numOfAttachments, attachments.data());
	//Unbind to prevent accidental changes
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
	//Delete framebuffer and its textures
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(m_attachmentID.size(), m_attachmentID.data());
}

void FBO::bind()
{
	//Bind the framebuffer and clear colour and depth
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBO::unbind()
{
	//Unbinds framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthMap::DepthMap(bool isShadow)
{
	//Generate a depth map framebuffer
	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER,m_ID);

	//Generate a texture using the depth component
	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	if(isShadow)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//There's no colour so I am not reading or drawing
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthMap::~DepthMap()
{
	//Delete framebuffer and textures
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(1, &m_depthMap);
}

void DepthMap::bind()
{
	//Bind and clear colour and depth
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void DepthMap::unbind()
{
	//Unbinds framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
