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

#include "PointLight.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fovy, const float &aspect, const float &near, const float &far) : ILight(POINT_LIGHT)
{
	_lights.push_back(std::shared_ptr<PerspectiveLight>(new PerspectiveLight(ambient, diffuse, pos, dir, up, fovy, aspect, near, far)));
	_lights.push_back(std::shared_ptr<PerspectiveLight>(new PerspectiveLight(ambient, diffuse, pos, glm::rotate(dir, glm::radians((float)-90), up), up, fovy, aspect, near, far)));
	_lights.push_back(std::shared_ptr<PerspectiveLight>(new PerspectiveLight(ambient, diffuse, pos, glm::rotate(dir, glm::radians((float)90), up), up, fovy, aspect, near, far)));
	_lights.push_back(std::shared_ptr<PerspectiveLight>(new PerspectiveLight(ambient, diffuse, pos, -dir, up, fovy, aspect, near, far)));
}

PointLight::~PointLight(void)
{
}

void			PointLight::setAmbient(float ambient)
{
	for (auto light = _lights.begin(); light != _lights.end(); light++)
		((PerspectiveLight *)(*light).get())->setAmbient(ambient);
}

void			PointLight::setAmbient(const glm::vec3 &ambient)
{
	for (auto light = _lights.begin(); light != _lights.end(); light++)
		((PerspectiveLight *)(*light).get())->setAmbient(ambient);
}

const glm::vec3	&PointLight::getAmbient(void) const
{
	return (_lights[0]->getAmbient());
}

void			PointLight::setDiffuse(float diffuse)
{
	for (auto light = _lights.begin(); light != _lights.end(); light++)
		((PerspectiveLight *)(*light).get())->setDiffuse(diffuse);
}

void			PointLight::setDiffuse(const glm::vec3 &diffuse)
{
	for (auto light = _lights.begin(); light != _lights.end(); light++)
		((PerspectiveLight *)(*light).get())->setDiffuse(diffuse);
}

const glm::vec3	&PointLight::getDiffuse(void) const
{
	return (_lights[0]->getDiffuse());
}

void			PointLight::setPos(const glm::vec3 &pos)
{
	for (auto light = _lights.begin(); light != _lights.end(); light++)
		((PerspectiveLight *)(*light).get())->setPos(pos);
}

void			PointLight::setDir(const glm::vec3 &dir)
{
	for (auto light = _lights.begin(); light != _lights.end(); light++)
		((PerspectiveLight *)(*light).get())->setDir(dir);
}

void			PointLight::setUp(const glm::vec3 &up)
{
	for (auto light = _lights.begin(); light != _lights.end(); light++)
		((PerspectiveLight *)(*light).get())->setUp(up);
}

const glm::vec3	&PointLight::getPos(void) const
{
	return (_lights[0]->getPos());
}

const glm::vec3	&PointLight::getDir(void) const
{
	return (_lights[0]->getDir());
}

const glm::vec3	&PointLight::getUp(void) const
{
	return (_lights[0]->getUp());
}

const std::vector<std::shared_ptr<PerspectiveLight>>	&PointLight::getLights(void) const
{
	return (_lights);
}
