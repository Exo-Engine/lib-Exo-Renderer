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

#include "IBodyPartInstance.h"

#include "BodyPart.h"

using namespace	ExoRenderer;

IBodyPartInstance::IBodyPartInstance(void) :
	_model(nullptr),
	_bodyPart(nullptr),
	_parent(nullptr)
{
}

IBodyPartInstance::IBodyPartInstance(BodyPart* bodyPart, IModelInstance* model, IBodyPartInstance* parent) :
	_model(model),
	_bodyPart(bodyPart),
	_parent(parent)
{
	_matrix = bodyPart->getBone().getMatrix();
}

IBodyPartInstance::~IBodyPartInstance(void)
{
}

IModelInstance*		IBodyPartInstance::getModel(void) const
{
	return (_model);
}

BodyPart*			IBodyPartInstance::getBodyPart(void) const
{
	return (_bodyPart);
}

IBodyPartInstance*	IBodyPartInstance::getParent(void) const
{
	return (_parent);
}

const glm::mat4&	IBodyPartInstance::getMatrix(void) const
{
	return (_matrix);
}

void	IBodyPartInstance::addChild(IBodyPartInstance* child)
{
	_childs.push_back(child);
}

void	IBodyPartInstance::removeChild(IBodyPartInstance* child)
{
	std::vector<IBodyPartInstance*>::iterator	iterator;

	for (iterator = _childs.begin(); iterator != _childs.end(); iterator++)
		if (*iterator == child)
		{
			_childs.erase(iterator);
			break;
		}
}

const std::vector<IBodyPartInstance*>&	IBodyPartInstance::getChilds(void) const
{
	return (_childs);
}
