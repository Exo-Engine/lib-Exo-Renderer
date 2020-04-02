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

#include "Buffer.h"
#include "Texture.h"

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Buffer::Buffer(void)
: _id(0)
{	}

Buffer::Buffer(unsigned long count, unsigned int size, const void* data, BufferType type, BufferDraw usage, unsigned char attribArray, bool normalized)
: _id(0)
{
	initialize(count, size, data, type, usage, attribArray, normalized);
}

Buffer::~Buffer(void)
{
	switch (_type)
	{
		case BufferType::VERTEXARRAY:
			glDeleteVertexArrays(1, &_id);
			break;
		case BufferType::RENDERBUFFER:
			glDeleteRenderbuffers(1, &_id);
			break;
		default:
			glDeleteBuffers(1, &_id);
			break;
	}
}

void Buffer::initialize(unsigned long count, unsigned int size, const void* data, BufferType type, BufferDraw usage, unsigned char attribArray, bool normalized)
{
	_count = count;
	_type = type;

	switch (_type)
	{
		case BufferType::VERTEXARRAY:
			GL_CALL(glGenVertexArrays(1, &_id));
			GL_CALL(glBindVertexArray(_id));
			break;
		case BufferType::ARRAYBUFFER:
			GL_CALL(glGenBuffers(1, &_id));
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _id));
			GL_CALL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(GL_FLOAT), data, (usage == BufferDraw::STATIC ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)));

			GL_CALL(glEnableVertexAttribArray(attribArray));
			GL_CALL(glVertexAttribPointer(attribArray, size, GL_FLOAT, (normalized ? GL_TRUE : GL_FALSE), 0, (void*)0));
			break;
		case BufferType::INDEXBUFFER:
			GL_CALL(glGenBuffers(1, &_id));
			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
			GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GL_UNSIGNED_INT), data, (usage == BufferDraw::STATIC ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)));
			break;
		case BufferType::RENDERBUFFER:
			GL_CALL(glGenRenderbuffers(1, &_id));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, _id));
			break;
	}
}

void Buffer::updateSubData(unsigned long count, const void* data)
{
	if (_type == BufferType::ARRAYBUFFER || _type == BufferType::INDEXBUFFER)
	{
		bind();
		GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(GL_FLOAT), data));
		unbind();
	}
}

void Buffer::bind(void) const
{
	switch (_type)
	{
		case BufferType::VERTEXARRAY:
			GL_CALL(glBindVertexArray(_id));
			break;
		case BufferType::ARRAYBUFFER:
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _id));
			break;
		case BufferType::INDEXBUFFER:
			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
			break;
		case BufferType::RENDERBUFFER:
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, _id));
			break;
	}
}

void Buffer::unbind(void) const
{
	switch (_type)
	{
		case BufferType::VERTEXARRAY:
			GL_CALL(glBindVertexArray(0));
			break;
		case BufferType::ARRAYBUFFER:
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
			break;
		case BufferType::INDEXBUFFER:
			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			break;
		case BufferType::RENDERBUFFER:
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
			break;
	}
}

// Getters
GLuint Buffer::getBuffer(void) const
{
	return _id;
}
