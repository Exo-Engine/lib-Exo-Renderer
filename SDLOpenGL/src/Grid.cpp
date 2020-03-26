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

#include "Grid.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Shader* Grid::pShader = nullptr;
Buffer* Grid::vaoBuffer = nullptr;
Buffer* Grid::vertexBuffer = nullptr;

Grid::Grid(int width, int height, glm::vec2 pos)
: _width(width), _height(height), _pos(pos)
{
}

Grid::~Grid(void)
{
}

void Grid::render(const glm::mat4& lookAt, const glm::mat4& perspective)
{
	vaoBuffer->bind();
	vertexBuffer->bind();
	
	pShader->bind();
	pShader->setMat4("projection", perspective);
	pShader->setMat4("view", lookAt);
	pShader->setVec4("color", 1, 1, 1, 1);
	
	// Loop - origin at center
	for (unsigned int x = 0; x < _width + 1; x++)
	{
		drawLine(x - (_width/2), _pos.y - (_height/2), 1.5708);
	}
	
	for (unsigned int y = 0; y < _height + 1; y++)
	{
		drawLine(_pos.x - (_width/2), y - (_height/2), 0.0f);
	}
}

void Grid::drawLine(int x, int y, float angle)
{
	static glm::mat4 model;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
	model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(_width, _height, 0.0f));
	pShader->setMat4("model", model);
	
	GL_CALL(glDrawArrays(GL_LINES, 0, 2));
}
