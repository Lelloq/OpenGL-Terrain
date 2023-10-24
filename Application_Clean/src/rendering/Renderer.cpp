/*\file Renderer.cpp*/
#include "rendering/Renderer.h"

//Binds vao and ebo and draws the buffers on screen
void Renderer::Draw(VAO& vao, EBO& ebo)
{
	vao.Bind();
	ebo.Bind();
	glDrawElements(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPatch(VAO& vao, EBO& ebo)
{
	vao.Bind();
	ebo.Bind();
	glDrawElements(GL_PATCHES, ebo.getCount(), GL_UNSIGNED_INT, 0);
}
