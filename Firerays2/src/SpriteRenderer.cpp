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

#include "SpriteRenderer.h"

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

Shader* SpriteRenderer::pShader = nullptr;
Buffer* SpriteRenderer::vaoBuffer = nullptr;
Buffer* SpriteRenderer::vertexBuffer = nullptr;
Buffer* SpriteRenderer::indexBuffer = nullptr;
Buffer* SpriteRenderer::uvBuffer = nullptr;

SpriteRenderer::SpriteRenderer(void)
: _pGrid(nullptr), _gridEnabled(false)
{
	_pGrid = new Grid(100, 100, {0.0f, 0.0f});
}

SpriteRenderer::~SpriteRenderer(void)
{
	if (_pGrid)
		delete _pGrid;
}

void SpriteRenderer::add(const sprite &s)
{
	_renderQueue.push_back(s);
}

void SpriteRenderer::remove(const sprite &s)
{
	for (std::deque<sprite>::iterator iterator = _renderQueue.begin(); iterator != _renderQueue.end(); iterator++)
	{
		if (&*iterator == &s)
		{
			_renderQueue.erase(iterator);
			return ;
		}
	}
}

void SpriteRenderer::render(Camera* camera, const glm::mat4& perspective)
{
	if (_gridEnabled)
		_pGrid->render(camera->getLookAt(), perspective);

	prepare(camera, perspective);

	for (sprite& object : _renderQueue)
	{
		renderSprite(object, pShader);
	}
}

void SpriteRenderer::setGrid(bool val)
{
	_gridEnabled = val;
}

// Private
void SpriteRenderer::prepare(Camera* camera, const glm::mat4& perspective)
{
	pShader->bind();
	pShader->setMat4("projection", perspective);
	pShader->setMat4("view", camera->getLookAt());

	// Render
	vaoBuffer->bind();
	vertexBuffer->bind();
	uvBuffer->bind();

	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void SpriteRenderer::renderSprite(sprite& s, Shader* shader)
{
	static glm::mat4 model;

	s.texture->bind();

	model = glm::translate(glm::mat4(1.0f), glm::vec3(s.position, 0.0f));
	model = glm::rotate(model, s.angle, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(s.scale, 0.0f));
	shader->setMat4("model", model);

	shader->setInt("layer", (int)s.layer);
	shader->setInt("flipHorizontal", s.flip == HORIZONTAL ? -1 : 1);
	shader->setInt("flipVertical", s.flip == VERTICAL ? -1 : 1);

	shader->setFloat("size", 1);

	GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));
}

