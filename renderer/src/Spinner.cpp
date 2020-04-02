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

#include "Spinner.h"

using namespace ExoRenderer;

Spinner::Spinner(const std::shared_ptr<ITexture> &texture, float scaleFactor, float contextWidth, float contextHeight)
: ISpinner(), _pTexture(texture), _rotation(0.0f)
{
	_scaleFactor = scaleFactor;
	_contextWidth = contextWidth;
	_contextHeight = contextHeight;
}

Spinner::~Spinner()
{	}

void Spinner::update(double deltaTime)
{
	_rotation += deltaTime * 0.004363323;
}

void Spinner::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType)
{
	(void)mouse;
	(void)keyboard;
	(void)gamepad;
}

// Getters
const std::shared_ptr<ITexture>& Spinner::getTexture()
{
	return _pTexture;
}

float Spinner::getRotation(void)
{
	return _rotation;
}

// Setters
void Spinner::setRotation(float rotation)
{
	_rotation = rotation;
}

void Spinner::setTexture(const std::shared_ptr<ITexture> &texture)
{
	_pTexture = texture;
}
