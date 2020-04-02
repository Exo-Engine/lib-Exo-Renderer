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

#include "Checkbox.h"

#include <iostream>

using namespace ExoRenderer;

Checkbox::Checkbox(const std::shared_ptr<ITexture>& texture, bool checked, float scaleFactor, float contextWidth, float contextHeight)
: ICheckbox(), _texture(texture), _checked(checked), _selected(false), _textureIndex(0)
{
	_scaleFactor = scaleFactor;
	_contextWidth = contextWidth;
	_contextHeight = contextHeight;

	// Cut texture
	if (_checked)
		setTextureIndex(2);
	else
		setTextureIndex(0);
}

Checkbox::~Checkbox(void)
{	}

void Checkbox::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType)
{
	(void)keyboard;
	(void)gamepad;

	if (_enabled)
	{
		if ((navigationType == NavigationType::MOUSE
			 && _realPosition.y + _virtualOffset.y + _relativeParentPosition.y + _scaleSize.y > mouse->y
			 && _realPosition.y + _virtualOffset.y + _relativeParentPosition.y - _scaleSize.y < mouse->y
			 && _realPosition.x + _virtualOffset.x + _relativeParentPosition.x + _scaleSize.x > mouse->x
			 && _realPosition.x + _virtualOffset.x + _relativeParentPosition.x - _scaleSize.x < mouse->x)
			|| _selected)
		{
			if ((mouse->isKeyDown(MouseButtons::BUTTON_LEFT) && !mouse->lastIsKeyDown(MouseButtons::BUTTON_LEFT))
				|| (gamepad->isKeyDown(GamepadButtons::BUTTON_A) && !gamepad->lastIsKeyDown(GamepadButtons::BUTTON_A)))
			{
				if (_textureIndex == 0 || _textureIndex == 1)
				{
					setTextureIndex(2);
					_checked = true;
				}
				else
				{
					setTextureIndex(0);
					_checked = false;
				}
				if (_callbackFunction) _callbackFunction(_callback);
			}
			else if (_textureIndex == 0)
				setTextureIndex(1);
			else if (_textureIndex == 2)
				setTextureIndex(3);
		}
		else if (_textureIndex == 1)
			setTextureIndex(0);
		else if (_textureIndex == 3)
			setTextureIndex(2);
	}
}

// Getters
bool Checkbox::getChecked(void)
{
	return _checked;
}

const std::shared_ptr<ITexture>& Checkbox::getTexture()
{
	return _texture;
}

glm::vec2 Checkbox::getOffset(void)
{
	return _offset;
}

// Setters
void Checkbox::setTexture(const std::shared_ptr<ITexture>& texture)
{
	_texture = texture;
}

void Checkbox::setChecked(bool checked)
{
	_checked = checked;

	// Update
	if (_checked)
		setTextureIndex(1);
	else
		setTextureIndex(0);
}

void Checkbox::setSelected(bool selected)
{
	_selected = selected;

	if (_selected)
	{
		if (_textureIndex == 0)
			setTextureIndex(1);
		else if (_textureIndex == 2)
			setTextureIndex(3);
	}
	else
	{
		if (_textureIndex == 1)
			setTextureIndex(0);
		else if (_textureIndex == 3)
			setTextureIndex(2);
	}
}

// Private
void Checkbox::setTextureIndex(const unsigned int& textureIndex)
{
	_textureIndex = textureIndex;
	_offset = glm::vec2(0, (_textureIndex % 4) / 4.0f);
}
