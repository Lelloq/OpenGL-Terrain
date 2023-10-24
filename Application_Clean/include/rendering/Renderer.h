/*\file Renderer.h*/
#pragma once
#include "VAO.h"
#include "EBO.h"

/*\class Renderer
Abstract class of renderer*/
class Renderer
{
public:
	virtual void Draw() const = 0;
	static void Draw(VAO& vao, EBO& ebo);
	static void DrawPatch(VAO& vao, EBO& ebo);
};