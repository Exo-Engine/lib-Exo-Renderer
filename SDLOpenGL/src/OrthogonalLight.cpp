/*
 *  MIT License
 *  
 *  Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "OrthogonalLight.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

OrthogonalLight::OrthogonalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const glm::vec2 &x, const glm::vec2 &y, const glm::vec2 &z) : Light(ambient, diffuse, pos, dir, up), _x(x), _y(y), _z(z)
{
	renderProjection();
}

OrthogonalLight::~OrthogonalLight(void)
{
}

void	OrthogonalLight::setX(const glm::vec2 &x)
{
	_x = x;
	renderProjection();
}

void	OrthogonalLight::setY(const glm::vec2 &y)
{
	_y = y;
	renderProjection();
}

void	OrthogonalLight::setZ(const glm::vec2 &z)
{
	_z = z;
	renderProjection();
}

const glm::vec2	&OrthogonalLight::getX(void) const
{
	return (_x);
}

const glm::vec2	&OrthogonalLight::getY(void) const
{
	return (_y);
}

const glm::vec2	&OrthogonalLight::getZ(void) const
{
	return (_z);
}

const glm::mat4	&OrthogonalLight::getProjection(void) const
{
	return (_projection);
}

void	OrthogonalLight::renderProjection(void)
{
	_radius = _z.y;
	_projection = glm::ortho(_x.x, _x.y, _y.x, _y.y, _z.x, _z.y);
}
