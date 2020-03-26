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

#include "Enums.h"
#include "IFrameBuffer.h"

namespace	ExoRenderer
{

class IWindow
{
public:
	IWindow(void): _width(0), _height(0), _contextWidth(0), _contextHeight(0), _highDPIFactor(1), _close(false)
	{ };
	
	virtual ~IWindow(void)
	{ };
	
	// Setters
	virtual void setWindowSize(int w, int h) = 0;
	virtual void setWindowMode(const WindowMode &mode) = 0;
	virtual void setVsync(bool vsync) = 0;

	// Getters
	virtual double getDelta(void) const = 0;
	virtual float getWidth(void) const = 0;
	virtual float getHeight(void) const = 0;
	
	virtual int getContextWidth(void) const = 0;
	virtual int getContextHeight(void) const = 0;
	
	virtual int getHighDPIFactor(void) const = 0;
	virtual bool isFullscreen(void) const = 0;
	
	virtual bool getIsClosing(void) const = 0;

	virtual IFrameBuffer *getFrameBuffer(void) const = 0;
protected:
	uint64_t		_now, _last;
	int			 _width, _height;
	WindowMode		_windowMode;
	int			 _contextWidth, _contextHeight, _highDPIFactor;
	bool			_close;
};

}
