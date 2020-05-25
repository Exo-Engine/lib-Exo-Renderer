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

#include "OGLException.h"

#include <utility>

using namespace ExoRendererFirerays2;

OGLException::OGLException(GLenum error)
{
}

OGLException::~OGLException(void)
{
}

static std::pair<GLenum, const char*>	openglErrors[] = {
	{GL_NO_ERROR, "GL_NO_ERROR"},
	{GL_INVALID_ENUM, "GL_INVALID_ENUM"},
	{GL_INVALID_VALUE, "GL_INVALID_VALUE"},
	{GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
	{GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"},
	{GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
	{GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
	{GL_TRUE, "GL_TRUE"},
	{GL_FALSE, "GL_FALSE"}
};

const char*	OGLException::what() const noexcept
{
	for (const std::pair<GLenum, const char*>& openglError : openglErrors)
		if (openglError.first == _error)
			return (openglError.second);
	return ("UNKNOWN");
}

OGLException	&OGLException::operator=(const std::exception &)
{
	return (*this);
}
