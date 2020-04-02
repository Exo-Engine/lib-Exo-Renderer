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

#include "Input.h"
#include "Button.h"
#include <cctype>

using namespace ExoRenderer;

Input::Input(const InputType &type, const std::shared_ptr<ITexture> &texture, const std::string &text, float scaleFactor, float contextWidth, float contextHeight)
: IInput(), _type(type), _min(-FLT_MAX), _max(FLT_MAX), _maxLength(100), _textureIndex(0), _pLabel(nullptr), _sliced(false), _numberOfRows(1), _numberOfColumns(1), _isSelected(false), _text(text), _positionInLabel(0), _pTexture(texture)
{
	_scaleFactor = scaleFactor;
	_contextWidth = contextWidth;
	_contextHeight = contextHeight;

	_pLabel = new Label();
	_pLabel->setScale(false);
	_pLabel->setText(text);
	_pLabel->setAnchor(_anchor);
	_pLabel->setLocalAnchor(AnchorPoint::CENTER_LEFT);
	_pLabel->setTextAlignment(TextAlignment::LEFT);
}

Input::~Input(void)
{
	if (_pLabel)
		delete _pLabel;
}

void Input::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType)
{
	(void)gamepad;

	if (_enabled &&
		_realPosition.y + _relativeParentPosition.y + _virtualOffset.y + _scaleSize.y > mouse->y && _realPosition.y + _relativeParentPosition.y + _virtualOffset.y - _scaleSize.y < mouse->y &&
		_realPosition.x + _relativeParentPosition.x + _virtualOffset.x + _scaleSize.x > mouse->x && _realPosition.x + _relativeParentPosition.x + _virtualOffset.x - _scaleSize.x < mouse->x)
	{
		if (mouse->isKeyDown(MouseButtons::BUTTON_LEFT)
			&& !mouse->lastIsKeyDown(MouseButtons::BUTTON_LEFT)
			&& _isSelected == false)
		{
			_isSelected = true;

			_text.append("|");
			_pLabel->setText(_text);
			_pLabel->updateGlyph();
			_positionInLabel = (unsigned int)_text.length() - 1;
		}
	}
	else if (_isSelected && !mouse->isKeyDown(MouseButtons::BUTTON_LEFT)
			 && mouse->lastIsKeyDown(MouseButtons::BUTTON_LEFT))
	{
		_isSelected = false;

		if (_text.length() > 0)
			_text.erase(_positionInLabel, 1);

		_positionInLabel = 0;
		overflow();
	}

	// When selected
	if (_isSelected)
	{
		if (keyboard->isKeyDown(KeyboardKeys::KEY_BACKSPACE)
			&& !keyboard->lastIsKeyDown(KeyboardKeys::KEY_BACKSPACE)
			&& _pLabel->getText().length() > 0 && _positionInLabel > 0) // Delete
		{
			_text.erase(_positionInLabel - 1, 1);
			_positionInLabel--;
			overflow();
		}
		else if (keyboard->isKeyDown(KeyboardKeys::KEY_RETURN))
		{
			if (_callbackFunction)
				_callbackFunction(_callback);
		}
		else if(keyboard->isKeyDown(KeyboardKeys::KEY_LEFT)
				&& !keyboard->lastIsKeyDown(KeyboardKeys::KEY_LEFT)
				&& _positionInLabel > 0) // Left
		{
			_text.erase(_positionInLabel, 1);
			_text.insert(_positionInLabel - 1, "|");
			_positionInLabel--;
			overflow();
		}
		else if(keyboard->isKeyDown(KeyboardKeys::KEY_RIGHT)
				&& !keyboard->lastIsKeyDown(KeyboardKeys::KEY_RIGHT)
				&& _positionInLabel+1 < _pLabel->getText().length()) // Left
		{
			_text.erase(_positionInLabel, 1);
			_text.insert(_positionInLabel + 1, "|");
			_positionInLabel++;
			overflow();
		}
		else
		{
			for (unsigned int i = 0; i < KeyboardKeys::KEY_MAX; i++)
			{
				if (keyboard->isKeyDown((KeyboardKeys)i) && !keyboard->lastIsKeyDown((KeyboardKeys)i))
				{
					const char* c = keyboard->convertKeyboardKeyToChar((KeyboardKeys)i);
					if (strlen(c) == 1)
					{
						if (_type == InputType::TEXT && *c >= 32 && *c <= 90 && _text.length() <= _maxLength)
							insertCharacter(keyboard, c);
						else if (_type == InputType::INTEGER || _type == InputType::DOUBLE)
						{
							if (_positionInLabel == 0 && *c == 45)
								insertCharacter(keyboard, c);
							else if (_type == InputType::DOUBLE)
							{
								if (*c == 46 || *c == 44)
								{
									const char* tc = ".";
									insertCharacter(keyboard, tc);
								}
							}
							else if (*c >= 48 && *c <= 57)
							{
								insertCharacter(keyboard, c);

								if (std::atoi(_text.c_str()) > _max)
								{
									_text = std::to_string((int)_max);
									 overflow();
									_positionInLabel = (unsigned int)_text.length();
								}
								else if (std::atoi(_text.c_str()) < _min)
								{
									_text = std::to_string((int)_min);
									 overflow();
									_positionInLabel = (unsigned int)_text.length();
								}
							}
						}
					}
				}
			}
		}
	}
}

void Input::updateLabel(void)
{
	if (_pLabel)
	{
		switch (_pLabel->getTextAlignment())
		{
			case TextAlignment::LEFT:
				_pLabel->setPosition(_realPosition.x - getScaleSize().x, _realPosition.y);
				break;
			case TextAlignment::RIGHT:
				_pLabel->setPosition(_realPosition.x + getScaleSize().x, _realPosition.y);
				break;
			default:
				_pLabel->setPosition(_realPosition);
				break; // Center
		}
		_pLabel->setVirtualOffset(_virtualOffset);
		_pLabel->setRelativeParentPosition(_relativeParentPosition);
	}
}

// Getters
InputType &Input::getType(void)
{
	return _type;
}

bool Input::getSliced(void)
{
	return _sliced;
}

bool Input::getSelected(void)
{
	return _isSelected;
}

const std::shared_ptr<ITexture>& Input::getTexture()
{
	return _pTexture;
}

std::string Input::getText()
{
	if (_text.back() == '|')
		return _text.erase(_text.length() - 1, 1);
	else
		return _text;
}

Label* Input::getLabel(void)
{
	return _pLabel;
}

unsigned int Input::getNumberOfRows(void)
{
	return _numberOfRows;
}

unsigned int Input::getNumberOfColumns(void)
{
	return _numberOfColumns;
}

unsigned int Input::getTextureIndex(void)
{
	return _textureIndex;
}

// Setters
void Input::setType(const InputType &type)
{
	_type = type;
}

void Input::setMax(float max)
{
	_max = max;
}

void Input::setMin(float min)
{
	_min = min;
}

void Input::setMaxLength(int max)
{
	_maxLength = max;
}

void Input::setSliced(bool sliced)
{
	_sliced = sliced;
}

void Input::setSelected(bool selected)
{
	_isSelected = selected;

	if (_isSelected)
	{
		_text.append("|");
		_positionInLabel = (unsigned int)_text.length() - 1;
	}
	else
	{
		_text.erase(_positionInLabel, 1);
		_pLabel->setText(_text);
		_pLabel->updateGlyph();
	}
}

void Input::setTexture(const std::shared_ptr<ITexture>& texture)
{
	_pTexture = texture;
}

void Input::setTextureCut(unsigned int numberOfRows, unsigned int numberOfColumns)
{
	_numberOfRows = numberOfRows;
	_numberOfColumns = numberOfColumns;
}

void Input::setFont(const std::shared_ptr<Font>& font)
{
	_pLabel->setFont(font);
}

void Input::setText(const std::string& text)
{
	_positionInLabel = 0;
	_isSelected = false;

	_text = text;
	_pLabel->setText(text);
}

void Input::setTextScale(float scale)
{
	_pLabel->setFontScale(scale);
}

void Input::setPadding(const glm::vec2& padding)
{
	setPadding(padding.x, padding.y);
}

void Input::setPadding(float x, float y)
{
	_pLabel->setMargin(x, y);
}

void Input::setColor(const glm::vec3& color)
{
	setColor(color.x, color.y, color.z);
}

void Input::setColor(float r, float g, float b)
{
	_pLabel->setColor(r, g, b);
}

// Private
void Input::insertCharacter(IKeyboard* keyboard, const char *c)
{
	_text.erase(_positionInLabel, 1);
	if (keyboard->isKeyDown(KeyboardKeys::KEY_LSHIFT))
		_text.insert(_positionInLabel, 1, std::toupper(*c));
	else
		_text.insert(_positionInLabel, 1, std::tolower(*c));

	_positionInLabel++;
	_text.insert(_positionInLabel, "|");
	overflow();
}

void Input::overflow()
{
	_pLabel->setText(_text);
	_pLabel->updateGlyph();
	if (_pLabel->getGlyphLayout().x > (_scaleSize.x * 2.0f) - 10)
	{
		if (_positionInLabel > _text.length() / 2)
		{
			while (_pLabel->getGlyphLayout().x > (_scaleSize.x * 2.0f) - 10)
			{
				_pLabel->setText(_pLabel->getText().erase(0, 1));
				_pLabel->updateGlyph();
			}
		}
		else
		{
			while (_pLabel->getGlyphLayout().x > (_scaleSize.x * 2.0f) - 10)
			{
				_pLabel->setText(_pLabel->getText().erase(_pLabel->getText().length()-1, 1));
				_pLabel->updateGlyph();
			}
		}
	}
}
