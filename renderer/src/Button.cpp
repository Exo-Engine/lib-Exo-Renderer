/*
 *  MIT License
 *  
 *  Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "Button.h"

using namespace ExoRenderer;

Button::Button(const std::shared_ptr<ITexture>& texture, ButtonType buttonType, bool withLabel, float scaleFactor, float contextWidth, float contextHeight)
: IButton(), _clickEvent(false), _pLabel(nullptr), _buttonType(buttonType), _sliced(false), _state(ButtonState::DEFAULT), _texture(texture), _textureIndex(0), _numberOfRows(1), _numberOfColumns(1), _defaultTextColor(0, 0, 0), _secondTextColor(0, 0, 0)
{
    _scaleFactor = scaleFactor;
    _contextWidth = contextWidth;
    _contextHeight = contextHeight;
    
    if (withLabel)
    {
        _pLabel = new Label();
        _pLabel->setScale(false);
    }
}

Button::~Button(void)
{
    if (_pLabel)
        delete _pLabel;
}

void Button::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType)
{
    (void)keyboard;
    
    if (_enabled)
    {
        if (navigationType == NavigationType::MOUSE && getRealPosition().y + _virtualOffset.y + _relativeParentPosition.y + getScaleSize().y > mouse->y // Y
            && getRealPosition().y + _virtualOffset.y + _relativeParentPosition.y - getScaleSize().y < mouse->y
            && getRealPosition().x + _virtualOffset.x + _relativeParentPosition.x + getScaleSize().x > mouse->x // X
            && getRealPosition().x + _virtualOffset.x + _relativeParentPosition.x - getScaleSize().x < mouse->x)
        {
            if (_state == ButtonState::DEFAULT && mouse->isKeyDown(MouseButtons::BUTTON_LEFT) == false)
            {
                _state = ButtonState::HOVER;
                if (_pLabel)
                    _pLabel->setColor(_secondTextColor);
                setTextureIndex(1);
            }
            else
            {
                switch (_buttonType) {
                    case ButtonType::NORMAL:
                        if (_state == ButtonState::HOVER
                            && !mouse->isKeyDown(MouseButtons::BUTTON_LEFT)
                            && mouse->lastIsKeyDown(MouseButtons::BUTTON_LEFT))
                        {
                            _state = ButtonState::DEFAULT;
                            if (_callbackFunction) _callbackFunction(_callback);
                            _clickEvent = true;
                            return;
                        }
                        break;
                    case ButtonType::DRAGGABLE:
                        if (mouse->isKeyDown(MouseButtons::BUTTON_LEFT))
                        {
                            _state = ButtonState::SELECTED;
                            if (_callbackFunction) _callbackFunction(_callback);
                            _clickEvent = true;
                            return;
                        }
                        _state = ButtonState::DEFAULT;
                        break;
                };
            }
        }
        else if (_state == ButtonState::SELECTED)
        {
            if (!mouse->isKeyDown(MouseButtons::BUTTON_LEFT)
                && mouse->lastIsKeyDown(MouseButtons::BUTTON_LEFT))
                _state = ButtonState::DEFAULT;
            else
            {
                if (_callbackFunction) _callbackFunction(_callback);
                _clickEvent = true;
                return;
            }
        }
        else if (_state == ButtonState::ACTIVATE)
        {
            if (_pLabel)
                _pLabel->setColor(_secondTextColor);
            setTextureIndex(1);
        }
        else
        {
            _state = ButtonState::DEFAULT;
            if (_pLabel)
                _pLabel->setColor(_defaultTextColor);
            setTextureIndex(0);
        }
    }
    
    _clickEvent = false;
}

void Button::updateLabel(void)
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
        _pLabel->contextInfo(_scaleFactor, _contextWidth, _contextHeight, true);
    }
}

void Button::resetDefaultState(void)
{
    _state = ButtonState::DEFAULT;
    if (_pLabel)
        _pLabel->setColor(_defaultTextColor);
    setTextureIndex(0);
}

// Getters
bool Button::getClickEvent(void)
{
    return _clickEvent;
}

bool Button::getSliced(void)
{
    return _sliced;
}

ButtonState Button::getState(void)
{
    return _state;
}

const std::shared_ptr<ITexture>& Button::getTexture()
{
    return _texture;
}

Label* Button::getLabel(void)
{
    return _pLabel;
}

unsigned int Button::getNumberOfRows(void)
{
    return _numberOfRows;
}

unsigned int Button::getNumberOfColumns(void)
{
    return _numberOfColumns;
}

glm::vec2 Button::getOffset(void)
{
    return _offset;
}

unsigned int Button::getTextureIndex(void)
{
    return _textureIndex;
}

// Setters
void Button::setSliced(bool sliced)
{
    _sliced = sliced;
}

void Button::setTexture(const std::shared_ptr<ITexture>& texture)
{
    _texture = texture;
}

void Button::setTextureCut(unsigned int numberOfRows, unsigned int numberOfColumns)
{
    _numberOfRows = numberOfRows;
    _numberOfColumns = numberOfColumns;
    
    _offset = glm::vec2((float)_textureIndex / _numberOfRows, (_textureIndex % _numberOfColumns) / (float)_numberOfColumns);
}

void Button::setState(const ButtonState& state)
{
    _state = state;
    
    if (_state == ButtonState::DEFAULT)
        setTextureIndex(0);
}

void Button::setFont(const std::shared_ptr<Font>& font)
{
    if (_pLabel)
        _pLabel->setFont(font);
}

void Button::setText(const std::string& text)
{
    if (_pLabel)
        _pLabel->setText(text);
}

void Button::setTextScale(float scale)
{
    if (_pLabel)
        _pLabel->setFontScale(scale);
}

void Button::setPadding(const glm::vec2& padding)
{
    if (_pLabel)
        _pLabel->setMargin(padding);
}

void Button::setPadding(float x, float y)
{
    setPadding(glm::vec2(x, y));
}

void Button::setType(const ButtonType& buttonType)
{
    _buttonType = buttonType;
}

void Button::setColor(const glm::vec3& color)
{
    _defaultTextColor = color;
    if (_pLabel)
        _pLabel->setColor(_defaultTextColor);
}

void Button::setColor(float r, float g, float b)
{
    _defaultTextColor = glm::vec3(r, g, b);
    if (_pLabel)
        _pLabel->setColor(_defaultTextColor);
}

void Button::setSecondColor(const glm::vec3& color)
{
    _secondTextColor = color;
}

void Button::setSecondColor(float r, float g, float b)
{
    _secondTextColor = glm::vec3(r, g, b);
}

void Button::setTextAlignment(const TextAlignment& align)
{
    _pLabel->setTextAlignment(align);
    switch (align)
    {
        case TextAlignment::LEFT:
            _pLabel->setLocalAnchor(AnchorPoint::CENTER_LEFT);
            break;
        case TextAlignment::RIGHT:
            _pLabel->setLocalAnchor(AnchorPoint::CENTER_RIGHT);
            break;
        default:
            _pLabel->setLocalAnchor(AnchorPoint::CENTER);
            break; // Center
    }
}

// Private
void Button::setTextureIndex(const unsigned int& textureIndex)
{
    if (_textureIndex != textureIndex)
    {
        _textureIndex = textureIndex;
        
        if (!_sliced)
            _offset = glm::vec2((float)_textureIndex / _numberOfRows, (_textureIndex % _numberOfColumns) / (float)_numberOfColumns);
    }
}
