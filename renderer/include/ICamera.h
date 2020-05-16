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

#pragma once

#include <glm/glm.hpp>
#include "IMouse.h"
#include "IKeyboard.h"
#include "IGamepad.h"

namespace	ExoRenderer
{

class ICamera
{
public:
	ICamera(void): _pos(0.0f, 0.0f, 2.0f)
	{ };

	virtual ~ICamera(void)
	{ };

	// Getters
	const glm::vec3&	getPos(void) const { return _pos; }
	const glm::vec3&	getDir(void) const { return _dir; }
	const glm::vec3&	getUp(void) const { return _up; }

	// Setters
	void	setPos(const glm::vec3& pos) { _pos = pos; }
	void	setPos(float x, float y, float z) { _pos = glm::vec3(x, y, z); }
	void	setDir(const glm::vec3& dir) { _dir = dir; }
	void	setDir(float x, float y, float z) { _dir = glm::vec3(x, y, z); }
	void	setUp(const glm::vec3& up) { _up = up; }
	void	setUp(float x, float y, float z) { _up = glm::vec3(x, y, z); }
protected:
	glm::vec3	_pos;
	glm::vec3	_dir;
	glm::vec3	_up;
};

}
