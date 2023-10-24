/*\file FBO.h*/
#pragma once
#include "Shader.h"
#include <vector>

/*\class FBO
\brief framebuffer object class for OpenGL*/
class FBO
{
private:
	GLuint m_ID;//!<Framebuffer ID
	GLuint m_RBO;//!<Renderbuffer ID
	std::vector<GLuint> m_attachmentID;//!<Frame attachment ID for the texture
public:
	FBO(GLuint numOfAttachments = 1, bool hdr = false);//!< Constructor takes in number of attachments and high dynamic range
	~FBO();//!< Destructor
	void bind();//!< Bind framebuffer
	void unbind();//!< Unbind framebuffer
	inline GLuint getID(GLuint index) { return m_attachmentID[index]; };//!< Get the attachment ID
};

/*\class DepthMap
\brief DepthMap framebuffer used for shadow mapping*/
class DepthMap
{
private:
	GLuint m_ID;//!<Framebuffer ID
	GLuint m_depthMap;//!<Texture attachment to the framebuffer
public:
	DepthMap(bool isShadow = false);//!<Constructor
	~DepthMap();//!<Destructor
	void bind();//!<Bind depth framebuffer
	void unbind();//!<Unbind depth framebuffer
	inline GLuint getID() { return m_depthMap; }//!<Get attachment ID
};