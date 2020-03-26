/*
 *	MIT License
 *	
 *	Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *	
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *	
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *	
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#pragma once

#include <deque>

#include "Camera.h"
#include "Shader.h"
#include "Buffer.h"
#include "sprite.h"
#include "Grid.h"

#include "Axis.h"

namespace	ExoRendererSDLOpenGL
{

class ObjectRenderer
{
public:
	ObjectRenderer(void);
	virtual ~ObjectRenderer(void);
	
	void push(const ExoRenderer::sprite &s);
	void render(Camera* camera, const glm::mat4& perspective);
	
	// Setters
	void setGrid(bool val);
private:
	void prepare(Camera* camera, const glm::mat4& perspective);
	static void renderObject(ExoRenderer::sprite& s, Shader* shader);
public:
	static Shader* pShader;
	static Buffer* vaoBuffer;
	static Buffer* vertexBuffer;
	static Buffer* indexBuffer;
	static Buffer* uvBuffer;
private:
	bool _gridEnabled;
	bool _axisEnabled;
	
	std::vector<ExoRenderer::sprite> _renderQueue;
	Grid	*_pGrid;
};

}
