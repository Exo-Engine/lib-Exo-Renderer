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

#include "Image.h"

using namespace ExoRenderer;

Image::Image(const std::shared_ptr<ITexture> &texture, float scaleFactor, float contextWidth, float contextHeight)
: IImage(), _pTexture(texture), _textureIndex(0), _numberOfRows(1), _numberOfColumns(1), _offset(0, 0), _rotation(0.0f)
{
	_scaleFactor = scaleFactor;
	_contextWidth = contextWidth;
	_contextHeight = contextHeight;
}

Image::~Image(void)
{	}

void Image::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType)
{
	(void)mouse;
	(void)keyboard;
	(void)gamepad;
}

// Getters
const std::shared_ptr<ITexture>& Image::getTexture()
{
	return _pTexture;
}

unsigned int Image::getNumberOfRows(void)
{
	return _numberOfRows;
}

unsigned int Image::getNumberOfColumns(void)
{
	return _numberOfColumns;
}

glm::vec2 Image::getOffset(void)
{
	return _offset;
}

float Image::getRotation(void)
{
	return _rotation;
}

// Setters
void Image::setRotation(float rotation)
{
	_rotation = rotation;
}

void Image::setTexture(const std::shared_ptr<ITexture> &texture)
{
	_pTexture = texture;
}

void Image::setTextureCut(unsigned int numberOfRows, unsigned int numberOfColumns)
{
	_numberOfRows = numberOfRows;
	_numberOfColumns = numberOfColumns;
	
	_offset = glm::vec2(_textureIndex / _numberOfRows, (_textureIndex % _numberOfColumns) / (float)_numberOfColumns);
}

void Image::setTextureIndex(unsigned int index)
{
	_textureIndex = index;
	_offset = glm::vec2(_textureIndex / _numberOfRows, (_textureIndex % _numberOfColumns) / (float)_numberOfColumns);
}
