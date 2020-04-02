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

#include <glm/vec2.hpp>
#include <string.h>

#include "Enums.h"

#define GAMEPAD_DEAD_ZONE 8000

namespace	ExoRenderer
{

class IGamepad
{
public:
	IGamepad(void)
	: leftStick(0.0f, 0.0f), rightStick(0.0f, 0.0f), triggerLeft(0.0f), triggerRight(0.0f), _real(true)
	{
		for (unsigned int i{ 0 }; i < GAMEPAD_BUTTON_MAX; i++)
			_buffer[i] = false;
	}

	virtual ~IGamepad(void)
	{	}

	virtual void keyDown(const GamepadButtons &id) = 0;
	virtual void keyUp(const GamepadButtons &id) = 0;

	virtual bool isKeyDown(const GamepadButtons &id) const = 0;
	virtual bool lastIsKeyDown(const GamepadButtons &id) const = 0;

	void reset(void)
	{
		leftStick = {0, 0};
		rightStick = {0, 0};

		triggerLeft = 0.0f;
		triggerRight = 0.0f;

		for (unsigned int i{ 0 }; i < GAMEPAD_BUTTON_MAX; i++)
			_buffer[i] = false;

		updateLastBuffer();
	}

	void updateLastBuffer(void)
	{
		memcpy(_lastBuffer, _buffer, sizeof(_buffer));
	}

	void setIsGamepad(bool val) { _real = val; }
	bool IsGamepad(void) { return _real; }
protected:
	bool _buffer[GAMEPAD_BUTTON_MAX];
	bool _lastBuffer[GAMEPAD_BUTTON_MAX];
public:
	glm::vec2 leftStick, rightStick;
	float triggerLeft, triggerRight;
	bool _real;
};

}
