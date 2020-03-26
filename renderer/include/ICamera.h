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
	ICamera(void): _speed(0.008f), _position(0.0f, 0.0f, 2.0f), _pFollowedEntity(nullptr), _pFollowedEntitySize(nullptr)
	{ };

	virtual ~ICamera(void)
	{ };

	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad) = 0;
	void resetFollowedEntity(void) { _pFollowedEntity = nullptr; _pFollowedEntitySize = nullptr; }

	// Getters
	float getSpeed(void) const { return _speed; }
	glm::vec3 const& getPosition(void) const { return _position; }
	const glm::vec2 *getFollowedEntity(void) const { return _pFollowedEntity; }

	// Setters
	void setSpeed(float speed) { _speed = speed; }

	void setPosition(const glm::vec3& position) { _position = position; }
	void setPosition(float x, float y, float z) { _position = glm::vec3(x, y, z); }

	void setFollowedEntity(const glm::vec2 *entity, const glm::vec2 *entitySize)
	{
		_pFollowedEntity = entity;
		_pFollowedEntitySize = entitySize;
	}
protected:
	float _speed;
	glm::vec3 _position;
	const glm::vec2 *_pFollowedEntity, *_pFollowedEntitySize;
};

}
