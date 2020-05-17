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

#include "ObjectRenderer.h"

#include "BodyPart.h"
#include "OGLCall.h"

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Shader* ObjectRenderer::pShader = nullptr;

ObjectRenderer::ObjectRenderer(void)
{
}

ObjectRenderer::~ObjectRenderer(void)
{
}

void	ObjectRenderer::add(ModelInstance* object)
{
	_renderQueue.push_back(object);
}

void	ObjectRenderer::remove(ModelInstance* object)
{
	for (std::deque<ModelInstance*>::iterator iterator = _renderQueue.begin(); iterator != _renderQueue.end(); iterator++)
	{
		if (*iterator == object)
		{
			_renderQueue.erase(iterator);
			return ;
		}
	}
}

void	ObjectRenderer::render(Camera* camera, const glm::mat4& perspective)
{
	prepare(camera, perspective);

	for (ModelInstance* object : _renderQueue)
		renderObject(object, pShader);
}

void	ObjectRenderer::prepare(Camera* camera, const glm::mat4& perspective)
{
	pShader->bind();
	pShader->setMat4("projection", perspective);
	pShader->setMat4("view", camera->getLookAt());
	pShader->setVec3("cameraPos", camera->getPos());
	pShader->setVec3("lightPos", glm::vec3(0, 10, 0));
	pShader->setVec3("lightColor", glm::vec3(1, 1, 1));
	pShader->setVec3("lightIntensity", glm::vec3(0.42, 0.6, 0.2));

	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glEnable(GL_TEXTURE_2D));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GL_CALL(glDepthFunc(GL_LESS));
	GL_CALL(glEnable(GL_BLEND));
}

void	ObjectRenderer::renderObject(ModelInstance* object, Shader* shader)
{
	renderBodyPart(static_cast<BodyPartInstance*>(object->getBody()), shader);
}

void	ObjectRenderer::renderBodyPart(BodyPartInstance* bodyPart, Shader* shader)
{
	Material*	material = bodyPart->getBodyPart()->getMaterial();

	pShader->setFloat("specularExponent", material->getSpecularExponent());
	pShader->setMat4("model", bodyPart->getMatrix());

	bodyPart->getVao()->bind();

	bodyPart->getVertexBuffer()->bind();
	bodyPart->getTextureVertexBuffer()->bind();
	bodyPart->getNormalBuffer()->bind();

	bodyPart->getAmbientTexture()->bind(0);
	bodyPart->getDiffuseTexture()->bind(1);
	bodyPart->getSpecularTexture()->bind(2);

	GL_CALL(glDrawArrays(GL_TRIANGLES, 0, bodyPart->getVertexBuffer()->getCount() * 3));
	for (IBodyPartInstance* child : bodyPart->getChilds())
		renderBodyPart(static_cast<BodyPartInstance*>(child), shader);
}
