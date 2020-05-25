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

#include "FrameBuffer.h"
#include "Texture.h"
#include "RendererFirerays.h"
#include <stdexcept>

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

FrameBuffer::FrameBuffer(void) :
	_width(-1), _height(-1)
{
	GL_CALL(glGenFramebuffers(1, &_id));
}

FrameBuffer::~FrameBuffer(void)
{
	glDeleteFramebuffers(1, &_id);
}

void	FrameBuffer::attach(ITexture *texture)
{
	switch (((Texture *)texture)->getFormat())
	{
		case RGB:
		case RGBA:
			GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _id));
			GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->getEngineId(), 0));
			break;
		case DEPTH:
			GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _id));
			GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getEngineId(), 0));
			break;
	}
	if (_width == -1)
	{
		_width = texture->getWidth();
		_height = texture->getHeight();
	}
	else if (texture->getWidth() != _width ||
		texture->getHeight() != _height)
		throw (std::invalid_argument("texture size differes from framebuffer size"));
}

void	FrameBuffer::detach(ITexture *texture)
{
	switch (((Texture *)texture)->getFormat())
	{
		case RGB:
		case RGBA:
			GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _id));
			GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0));
			break;
		case DEPTH:
			GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _id));
			GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0));
			break;
	}
}

void	FrameBuffer::bind(void)
{
	if (_width == -1)
		throw (std::logic_error("no texture binded to framebuffer"));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, _id));
	GL_CALL(glViewport(0, 0, RendererFirerays::Get().getWindow()->getWidth(), RendererFirerays::Get().getWindow()->getHeight()));
}

void	FrameBuffer::unbind(void)
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void	FrameBuffer::clear(void)
{
	bind();
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
