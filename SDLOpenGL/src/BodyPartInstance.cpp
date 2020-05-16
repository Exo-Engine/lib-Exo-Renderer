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

#include "BodyPartInstance.h"

#include "BodyPart.h"

using namespace	ExoRenderer;
using namespace	ExoRendererSDLOpenGL;

BodyPartInstance::BodyPartInstance(void) :
	_vertexBuffer(nullptr),
	_textureVertexBuffer(nullptr),
	_normalBuffer(nullptr),
	_ambiantTexture(nullptr),
	_diffuseTexture(nullptr),
	_specularTexture(nullptr)
{
}

BodyPartInstance::BodyPartInstance(BodyPart* bodyPart, IModelInstance* model, BodyPartInstance* parent) :
	IBodyPartInstance(bodyPart, model, parent),
	_vertexBuffer(nullptr),
	_textureVertexBuffer(nullptr),
	_normalBuffer(nullptr),
	_ambiantTexture(nullptr),
	_diffuseTexture(nullptr),
	_specularTexture(nullptr)
{
}

BodyPartInstance::~BodyPartInstance(void)
{
}

void	BodyPartInstance::setVao(Buffer* buffer)
{
	_vao = buffer;
}

void	BodyPartInstance::setVertexBuffer(Buffer* buffer)
{
	_vertexBuffer = buffer;
}

void	BodyPartInstance::setTextureVertexBuffer(Buffer* buffer)
{
	_textureVertexBuffer = buffer;
}

void	BodyPartInstance::setNormalBuffer(Buffer* buffer)
{
	_normalBuffer = buffer;
}

void	BodyPartInstance::setAmbientTexture(Texture* texture)
{
	_ambiantTexture = texture;
}

void	BodyPartInstance::setDiffuseTexture(Texture* texture)
{
	_diffuseTexture = texture;
}

void	BodyPartInstance::setSpecularTexture(Texture* texture)
{
	_specularTexture = texture;
}

Buffer*	BodyPartInstance::getVao(void) const
{
	return (_vao);
}

Buffer*	BodyPartInstance::getVertexBuffer(void) const
{
	return (_vertexBuffer);
}

Buffer*	BodyPartInstance::getTextureVertexBuffer(void) const
{
	return (_textureVertexBuffer);
}

Buffer*	BodyPartInstance::getNormalBuffer(void) const
{
	return (_normalBuffer);
}

Texture*	BodyPartInstance::getAmbientTexture(void) const
{
	return (_ambiantTexture);
}

Texture*	BodyPartInstance::getDiffuseTexture(void) const
{
	return (_diffuseTexture);
}

Texture*	BodyPartInstance::getSpecularTexture(void) const
{
	return (_specularTexture);
}
