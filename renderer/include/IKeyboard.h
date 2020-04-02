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

#include <stdio.h>
#include <string.h>

#include "Enums.h"

namespace	ExoRenderer
{

class IKeyboard
{
public:
	IKeyboard(void)
	{
		for (unsigned int i{ 0 }; i < KEY_MAX; i++)
			_buffer[i] = false;
	}

	virtual ~IKeyboard(void)
	{	}

	virtual void keyDown(const KeyboardKeys &id) = 0;
	virtual void keyUp(const KeyboardKeys &id) = 0;

	virtual bool isKeyDown(const KeyboardKeys &id) const = 0;
	virtual bool lastIsKeyDown(const KeyboardKeys &id) const = 0;
	virtual const char* convertKeyboardKeyToChar(const KeyboardKeys &key) const = 0;

	void reset(void)
	{
		for (unsigned int i{ 0 }; i < KEY_MAX; i++)
			_buffer[i] = false;

		updateLastBuffer();
	}

	void updateLastBuffer(void)
	{
		memcpy(_lastBuffer, _buffer, sizeof(_buffer));
	}
protected:
	bool _buffer[KEY_MAX];
	bool _lastBuffer[KEY_MAX];
};

}
