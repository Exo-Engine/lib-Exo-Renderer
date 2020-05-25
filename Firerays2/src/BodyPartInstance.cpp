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

#include "RendererFirerays.h"
#include "BodyPart.h"

using namespace	ExoRenderer;
using namespace	ExoRendererFirerays2;

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

void				BodyPartInstance::setShape(RadeonRays::Shape* shape)
{
	_shape = shape;
}

RadeonRays::Shape*	BodyPartInstance::getShape(void) const
{
	return (_shape);
}

void		BodyPartInstance::translate(const glm::vec3& translation)
{
	IBodyPartInstance::translate(translation);
	// RadeonRays::matrix	matrix = toRRMatrix(getMatrix());
	RadeonRays::matrix	matrix, inv;
	_shape->GetTransform(matrix, inv);
	matrix *= RadeonRays::translation(RadeonRays::float3(translation.x, translation.y, translation.z));
	_shape->SetTransform(matrix, inverse(matrix));
	RendererFirerays::Get().changesMade();
}

void		BodyPartInstance::rotate(float angle, const glm::vec3& axis)
{
	IBodyPartInstance::rotate(angle, axis);
	// RadeonRays::matrix	matrix = toRRMatrix(getMatrix());
	RadeonRays::matrix	matrix, inv;
	_shape->GetTransform(matrix, inv);
	matrix *= RadeonRays::rotation(RadeonRays::float3(axis.x, axis.y, axis.z), angle);
	_shape->SetTransform(matrix, inverse(matrix));
	RendererFirerays::Get().changesMade();
}
