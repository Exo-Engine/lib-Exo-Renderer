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

#include "PerspectiveLight.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

PerspectiveLight::PerspectiveLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fovy, const float &aspect, const float &near, const float &far) : Light(ambient, diffuse, pos, dir, up), _fovy(fovy), _aspect(aspect), _near(near), _far(far)
{
	renderProjection();
}

PerspectiveLight::~PerspectiveLight(void)
{
}

void	PerspectiveLight::setFov(const float &fovy)
{
	_fovy = fovy;
	renderProjection();
}

void	PerspectiveLight::setAspect(const float &aspect)
{
	_aspect = aspect;
	renderProjection();
}

void	PerspectiveLight::setNear(const float &near)
{
	_near = near;
	renderProjection();
}

void	PerspectiveLight::setFar(const float &far)
{
	_far = far;
	renderProjection();
}

const float	&PerspectiveLight::getFov(void) const
{
	return (_fovy);
}

const float	&PerspectiveLight::getAspect(void) const
{
	return (_aspect);
}

const float	&PerspectiveLight::getNear(void) const
{
	return (_near);
}

const float	&PerspectiveLight::getFar(void) const
{
	return (_far);
}

const glm::mat4	&PerspectiveLight::getProjection(void) const
{
	return (_projection);
}

void	PerspectiveLight::renderProjection(void)
{
	_radius = _far;
	_projection = glm::perspective(_fovy, _aspect, _near, _far);
}
