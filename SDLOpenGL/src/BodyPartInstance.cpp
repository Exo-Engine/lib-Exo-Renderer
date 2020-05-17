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

BodyPartInstance::BodyPartInstance(void)
{
}

BodyPartInstance::BodyPartInstance(BodyPart* bodyPart, IModelInstance* model, BodyPartInstance* parent) :
	IBodyPartInstance(bodyPart, model, parent)
{
}

BodyPartInstance::~BodyPartInstance(void)
{
}

Buffer*	BodyPartInstance::getVao(void) const
{
	return (getBodyPart()->getData<t_bodyPartData>()->_vao);
}

Buffer*	BodyPartInstance::getVertexBuffer(void) const
{
	return (getBodyPart()->getData<t_bodyPartData>()->_vertexBuffer);
}

Buffer*	BodyPartInstance::getTextureVertexBuffer(void) const
{
	return (getBodyPart()->getData<t_bodyPartData>()->_textureVertexBuffer);
}

Buffer*	BodyPartInstance::getNormalBuffer(void) const
{
	return (getBodyPart()->getData<t_bodyPartData>()->_normalBuffer);
}

Texture*	BodyPartInstance::getAmbientTexture(void) const
{
	return (getBodyPart()->getData<t_bodyPartData>()->_ambiantTexture);
}

Texture*	BodyPartInstance::getDiffuseTexture(void) const
{
	return (getBodyPart()->getData<t_bodyPartData>()->_diffuseTexture);
}

Texture*	BodyPartInstance::getSpecularTexture(void) const
{
	return (getBodyPart()->getData<t_bodyPartData>()->_specularTexture);
}
