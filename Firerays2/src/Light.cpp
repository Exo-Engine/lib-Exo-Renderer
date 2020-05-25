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

#include "Light.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up) : ILight(DEFAULT_LIGHT), _ambient(ambient), _diffuse(diffuse), _pos(pos), _dir(dir), _up(up)
{
	renderViewMatrix();
}

Light::~Light(void)
{
}

void	Light::setAmbient(float ambient)
{
	_ambient = glm::vec3(ambient, ambient, ambient);
}

void	Light::setAmbient(const glm::vec3 &ambient)
{
	_ambient = ambient;
}

const glm::vec3	&Light::getAmbient(void) const
{
	return (_ambient);
}

void	Light::setDiffuse(float diffuse)
{
	_diffuse = glm::vec3(diffuse, diffuse, diffuse);
}

void	Light::setDiffuse(const glm::vec3 &diffuse)
{
	_diffuse = diffuse;
}

const glm::vec3	&Light::getDiffuse(void) const
{
	return (_diffuse);
}

const glm::vec3	&Light::getPos(void) const
{
	return (_pos);
}

const glm::vec3	&Light::getDir(void) const
{
	return (_dir);
}

const glm::vec3	&Light::getUp(void) const
{
	return (_up);
}

void	Light::setPos(const glm::vec3 &pos)
{
	_pos = pos;
	renderViewMatrix();
}

void	Light::setDir(const glm::vec3 &dir)
{
	_dir = dir;
	renderViewMatrix();
}

void	Light::setUp(const glm::vec3 &up)
{
	_up = up;
	renderViewMatrix();
}

const float	&Light::getRadius(void) const
{
	return (_radius);
}

const glm::mat4	&Light::getView(void) const
{
	return (_view);
}

void	Light::renderViewMatrix(void)
{
	_view = glm::lookAt(_pos, _pos + _dir, _up);
}
