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

#include "Axis.h"
#include "ObjectRenderer.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Shader* Axis::pShader = nullptr;
Buffer* Axis::vaoBuffer = nullptr;
Buffer* Axis::vertexBuffer = nullptr;

Axis::Axis(void)
: IAxis()
{
}

Axis::~Axis(void)
{
}

void Axis::render(const glm::mat4& lookAt, const glm::mat4& perspective)
{
	// X - Red
	drawAxis(1, 0, 0, glm::vec3(1, 0, 0), lookAt, perspective);
	
	// Y - Green
	drawAxis(0, 1, 1.5708, glm::vec3(0, 1, 0), lookAt, perspective);
}

// Private
void Axis::drawAxis(int x, int y, float angle, const glm::vec3 &color, const glm::mat4& lookAt, const glm::mat4& perspective)
{
	// Line
	Grid::vaoBuffer->bind();
	Grid::vertexBuffer->bind();
	
	Grid::pShader->bind();
	Grid::pShader->setMat4("projection", perspective);
	Grid::pShader->setMat4("view", lookAt);
	Grid::pShader->setVec4("color", glm::vec4(color, 1));
	
	static glm::mat4 model;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(_pos.x, _pos.y, 0.0f));
	model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
	Grid::pShader->setMat4("model", model);
	
	GL_CALL(glDrawArrays(GL_LINES, 0, 2));
	
	// Geometry
	pShader->bind();
	pShader->setMat4("projection", perspective);
	pShader->setMat4("view", lookAt);
	pShader->setVec4("color", glm::vec4(color, 1));
	
	switch (_type) {
		case AxisType::SCALE: {
			ObjectRenderer::vaoBuffer->bind();
			ObjectRenderer::vertexBuffer->bind();
			
			model = glm::translate(glm::mat4(1.0f), glm::vec3(_pos.x + x, _pos.y + y, 0.0f));
			model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.0f));
			pShader->setMat4("model", model);
			
			GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));
			break;
		}
		default: // Translation
			vaoBuffer->bind();
			vertexBuffer->bind();
			
			model = glm::translate(glm::mat4(1.0f), glm::vec3(_pos.x + x, _pos.y + y, 0.0f));
			model = glm::rotate(model, (angle - 1.5708f), glm::vec3(0, 0, 1));
			pShader->setMat4("model", model);
			
			GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
			break;
	}
}
