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
#include "OGLCall.h"

namespace	ExoRendererSDLOpenGL
{

class Buffer
{
public:
	Buffer(void);
	Buffer(unsigned long count, unsigned int size, const void* data, ExoRenderer::BufferType type, ExoRenderer::BufferDraw usage, unsigned char attribArray, bool normalized);
	virtual ~Buffer(void);
	
	virtual void initialize(unsigned long count, unsigned int size, const void* data, ExoRenderer::BufferType type, ExoRenderer::BufferDraw usage, unsigned char attribArray, bool normalized);
	virtual void updateSubData(unsigned long count, const void* data);
	
	virtual void bind(void) const;
	virtual void unbind(void) const;
	
	// Getters
	GLuint getBuffer(void) const;
private:
	unsigned long _count;
	ExoRenderer::BufferType _type;
	
	GLuint _id;
};

}
