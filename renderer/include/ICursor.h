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
#include "ITexture.h"
#include "IImage.h"
#include <memory>

namespace	ExoRenderer
{

enum class CursorState {
	DEFAULT,
	HIDDEN
};

class ICursor
{
public:
	ICursor()
	: _state(CursorState::DEFAULT), _pImage(nullptr)
	{ }

	virtual ~ICursor(void)
	{
		if (_pImage)
			delete _pImage;
	}

	virtual void update(void) = 0;

	// Setters
	void setState(const CursorState& state) { _state = state; }
	virtual void setCursorTexture(const std::shared_ptr<ITexture> &texture) = 0;
	IImage* getImage(void) { return _pImage; }

protected:
	CursorState _state;
	IImage* _pImage;
};

}
