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

#include "Slider.h"

using namespace ExoRenderer;

Slider::Slider(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& barTexture, float scaleFactor, float contextWidth, float contextHeight)
: ISlider(), _value(0.0f), _selected(false), _pBar(nullptr), _pButton(nullptr)
{
	_scaleFactor = scaleFactor;
	_contextWidth = contextWidth;
	_contextHeight = contextHeight;
	
	// Button
	_pButton = new Button(buttonTexture, ButtonType::DRAGGABLE, true, _scaleFactor, _contextWidth, _contextHeight);
	_pButton->setLocalAnchor(AnchorPoint::CENTER);
	_pButton->setSliced(true);
	
	// Bar (Background)
	_pBar = new Image(barTexture, _scaleFactor, _contextWidth, _contextHeight);
}

Slider::~Slider(void)
{
	if (_pBar)
		delete _pBar;
	
	if (_pButton)
		delete _pButton;
}

void Slider::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType)
{
	(void)keyboard;
	
	if (_enabled)
	{
		_pButton->update(mouse, keyboard, gamepad, navigationType);
		if (_pButton->getClickEvent())
		{
			_pButton->setPosition((mouse->x - _relativeParentPosition.x) / _scaleFactor, _pButton->getPosition().y);
			updatePosition();
		}
		
		if (_selected)
		{
			if (gamepad->rightStick.x > 0 + GAMEPAD_DEAD_ZONE)
			{
				_pButton->setPosition(_pButton->getPosition().x + 0.5, _pButton->getPosition().y);
				updatePosition();
			}
			else if (gamepad->rightStick.x < 0 - GAMEPAD_DEAD_ZONE)
			{
				_pButton->setPosition(_pButton->getPosition().x - 0.5, _pButton->getPosition().y);
				updatePosition();
			}
		}
	}
}

// Getters
float Slider::getValue(void) const
{
	return _value;
}

Button* Slider::getSliderButton(void)
{
	return _pButton;
}

Image* Slider::getBarImage(void)
{
	return _pBar;
}

// Setters
void Slider::setValue(float value)
{
	_value = value;
	
	// Update button
	_pButton->setPosition(getUnscaleRealPosition().x - _size.x + (((_size.x * 2) / 100) * _value), getUnscaleRealPosition().y);
}

void Slider::setSelected(bool selected)
{
	_selected = selected;
	
	if (_selected)
		_pButton->setState(ButtonState::ACTIVATE);
	else
		_pButton->setState(ButtonState::DEFAULT);
}

// Private
void Slider::updatePosition(void)
{
	// Limitations
	if (_pButton->getUnscaleRealPosition().x > getUnscaleRealPosition().x + _size.x)
		_pButton->setPosition(getUnscaleRealPosition().x + _size.x, _pButton->getPosition().y);
	else if (_pButton->getUnscaleRealPosition().x < getUnscaleRealPosition().x - _size.x)
		_pButton->setPosition(getUnscaleRealPosition().x - _size.x, _pButton->getPosition().y);
	
	_value = ((_pButton->getUnscaleRealPosition().x - (getUnscaleRealPosition().x - _size.x)) * 100) / (_size.x * 2);
	if (_callbackFunction) _callbackFunction(_callback);
}
