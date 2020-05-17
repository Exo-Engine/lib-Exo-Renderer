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

#include "BodyPart.h"

using namespace	ExoRenderer;

Bone::Bone(void) :
	_matrix(1)
{
}

Bone::Bone(const Bone& src)
{
}

Bone::~Bone(void)
{
}

Bone&	Bone::operator=(const Bone& src)
{
}

const glm::mat4&	Bone::getMatrix(void) const
{
	return (_matrix);
}

BodyPart::BodyPart(void) :
	_parent(nullptr),
	_material(nullptr)
{
}

BodyPart::BodyPart(Model* model, BodyPart* parent) :
	_model(model),
	_parent(parent)
{
	if (_parent)
		_parent->addChild(this);
}

BodyPart::BodyPart(const BodyPart& src)
{
}

BodyPart::~BodyPart(void)
{
}

void	BodyPart::addFace(const Face& face)
{
	_vertices.push_back(face.vertices);
	_textureVertices.push_back(face.textureVertices);
	_verticesnormals.push_back(face.verticesnormals);
}

void	BodyPart::addChild(BodyPart* bodyPart)
{
	_childs.push_back(bodyPart);
}

void	BodyPart::setMaterial(Material* material)
{
	_material = material;
}

Model*		BodyPart::getModel(void) const
{
	return (_model);
}

Bone&		BodyPart::getBone(void)
{
	return (_bone);
}

const Bone&	BodyPart::getBone(void) const
{
	return (_bone);
}

BodyPart*								BodyPart::getParent(void) const
{
	return (_parent);
}

Material*								BodyPart::getMaterial(void) const
{
	return (_material);
}

const std::vector<BodyPart*>&			BodyPart::getChilds(void) const
{
	return (_childs);
}

const std::vector<glm::highp_uvec3>&	BodyPart::getVertices(void) const
{
	return (_vertices);
}

const std::vector<glm::highp_uvec3>&	BodyPart::getVerticesNormals(void) const
{
	return (_verticesnormals);
}

const std::vector<glm::highp_uvec3>&	BodyPart::getTextureVertices(void) const
{
	return (_textureVertices);
}

BodyPart&	BodyPart::operator=(const BodyPart& src)
{
}
