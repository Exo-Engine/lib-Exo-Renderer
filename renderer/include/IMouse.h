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
#include <cstring>
#include "Enums.h"

namespace	ExoRenderer
{

class IMouse
{
public:
	IMouse(void)
	: x(0), y(0), wheelX(0), wheelY(0)
	{
		for (unsigned int i{ 0 }; i < MOUSE_BUTTON_MAX; i++)
			_buffer[i] = false;
	}
	
	virtual ~IMouse(void)
	{	}
	
	virtual void keyDown(const MouseButtons &id) = 0;
	virtual void keyUp(const MouseButtons &id) = 0;
	
	virtual bool isKeyDown(const MouseButtons &id) const = 0;
	virtual bool lastIsKeyDown(const MouseButtons &id) const = 0;
	
	void reset(void)
	{
		for (unsigned int i{ 0 }; i < MOUSE_BUTTON_MAX; i++)
			_buffer[i] = false;
		
		x = 0;
		y = 0;
		updateLastBuffer();
	}
	
	void updateLastBuffer(void)
	{
		memcpy(_lastBuffer, _buffer, sizeof(_buffer));
		
		// Position
		lastX = x;
		lastY = y;
		
		// Reset wheel, data per frame
		wheelX = 0;
		wheelY = 0;
	}
public:
	uint16_t x, y;
	uint16_t lastX, lastY;
	int wheelX, wheelY;
protected:
	bool _buffer[MOUSE_BUTTON_MAX];
	bool _lastBuffer[MOUSE_BUTTON_MAX];
};

}
