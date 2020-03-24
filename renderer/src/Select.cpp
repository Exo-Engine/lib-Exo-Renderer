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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>

#include "Select.h"
#include "Navigator.h"
#include <iostream>

using namespace ExoRenderer;

Select::Select(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, const std::shared_ptr<Font>& font, float scaleFactor, float contextWidth, float contextHeight)
: ISelect(), _state(SelectState::DEFAULT), _isOpen(false), _selectedId(-1), _pButton(nullptr), _pView(nullptr), _buttonTexture(buttonTexture), _font(font)
{
    _scaleFactor = scaleFactor;
    _contextWidth = contextWidth;
    _contextHeight = contextHeight;
    
    _pButton = new Button(buttonTexture, ButtonType::NORMAL, true, _scaleFactor, _contextWidth, _contextHeight);
    _pButton->setSliced(true);
    
    _pButton->setFont(_font);
    _pButton->setTextScale(0.25f);
    _pButton->setPadding(0, -3);
    _pButton->setColor(0, 0, 0);
    _pButton->setSecondColor(0, 0, 0);

    _pView = new View(backgroundTexture, scrollTexture, 3, 6, _scaleFactor, _contextWidth, _contextHeight);
    _pView->setAnchor(AnchorPoint::TOP_LEFT);
}

Select::~Select(void)
{
    for(auto& option : _options)
        delete option;
    _options.clear();
    
    for(auto& button : _buttonQueue)
        delete button;
    _buttonQueue.clear();
    
    if (_pButton)
        delete _pButton;
    
    if (_pView)
        delete _pView;
}

void Select::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType)
{
    _pButton->update(mouse, keyboard, gamepad, navigationType);
    if (_pButton->getClickEvent() || (_state == SelectState::ACTIVATE && gamepad->isKeyDown(GamepadButtons::BUTTON_A) && !gamepad->lastIsKeyDown(GamepadButtons::BUTTON_A)))
    {
        _isOpen = !_isOpen;
        if (_callbackFunction) _callbackFunction(_callback);
        
        if (_isOpen)
        {
            _pView->setState(ViewState::SELECTED);
            if (_pNavigator && _buttonQueue.size() > 0)
            {
                if (_selectedId == 0)
                    _pNavigator->updateSelectedWidget(_buttonQueue[1], gamepad, navigationType);
                else
                    _pNavigator->updateSelectedWidget(_buttonQueue[0], gamepad, navigationType);
            }
        }
        else
        {
            _pView->setState(ViewState::DEFAULT);
            if (_pNavigator)
                _pNavigator->updateSelectedWidget(this, gamepad, navigationType);
        }
    }
    
    if (_isOpen)
    {
        // If user click outside, close
        if (!(_realPosition.y + _virtualOffset.y + _relativeParentPosition.y + _scaleSize.y + (_pView->getScaleSize().y * 2) > mouse->y // Y
            && _realPosition.y + _virtualOffset.y + _relativeParentPosition.y - _scaleSize.y < mouse->y
            && _realPosition.x + _virtualOffset.x + _relativeParentPosition.x + _scaleSize.x + 15.0f > mouse->x // X
            && _realPosition.x + _virtualOffset.x + _relativeParentPosition.x - _scaleSize.x < mouse->x)
            && mouse->isKeyDown(MouseButtons::BUTTON_LEFT)
            && !mouse->lastIsKeyDown(MouseButtons::BUTTON_LEFT))
        {
            _isOpen = false;
        }
        
        for(int i = 0; i < (int)_buttonQueue.size(); i++)
        {
            if (i != _selectedId)
                _pView->addChild(_buttonQueue[i]);
        }
        
        int y = 0;
        for(int i = 0; i < (int)_buttonQueue.size(); i++)
        {
            if (i != _selectedId)
            {
                _buttonQueue[i]->setPosition(0, y);
                _buttonQueue[i]->update(mouse, keyboard, gamepad, navigationType);
                if (_buttonQueue[i]->getClickEvent())
                {
                    if (_callbackFunction) _callbackFunction(_callback);
                    _selectedId = i;
                    _pButton->setText(_options[_selectedId]->text);
                    
                    _isOpen = false;
                    _pView->setState(ViewState::DEFAULT);
                    if (_pNavigator)
                        _pNavigator->updateSelectedWidget(this, gamepad, navigationType);
                    
                    updateNavigation();
                    mouse->reset();
                }
                
                y += _size.y * 2;
            }
        }
        
        _pView->setSize(_size.x, _size.y * _buttonQueue.size() > SELECT_VIEW_MAX_HEIGHT ? SELECT_VIEW_MAX_HEIGHT : _size.y * _buttonQueue.size());
        _pView->setRelativeParentPosition(getRealPosition().x + _relativeParentPosition.x + _virtualOffset.x, getRealPosition().y + _relativeParentPosition.y + _virtualOffset.y + (_pView->getSize().y * _scaleFactor) + getScaleSize().y);
        
        _pView->update(mouse, keyboard, gamepad, navigationType);
    }
}

void Select::updateNavigation(void)
{
    // After because they don't exist
    for (int i = 0; i < _buttonQueue.size(); i++)
    {
        if (i != _selectedId)
        {
            // Top
            if (i - 1 == _selectedId)
                _buttonQueue[i]->setNextWidget(WidgetNext::TOP, nullptr);
            else if (i - 1 >= 0)
                _buttonQueue[i]->setNextWidget(WidgetNext::TOP, _buttonQueue[i - 1]);
            
            // Bottom
            if (i + 1 == _selectedId)
                _buttonQueue[i]->setNextWidget(WidgetNext::BOTTOM, nullptr);
            else if (i + 1 < (int)_buttonQueue.size())
                _buttonQueue[i]->setNextWidget(WidgetNext::BOTTOM, _buttonQueue[i + 1]);
        }
        else
        {
            // Top
            if (i - 2 >= 0)
                _buttonQueue[i - 1]->setNextWidget(WidgetNext::TOP, _buttonQueue[i - 2]);
            
            // Bottom
            if (i + 1 < (int)_buttonQueue.size() && i - 1 >= 0)
                _buttonQueue[i - 1]->setNextWidget(WidgetNext::BOTTOM, _buttonQueue[i + 1]);
        }
    }
}

void Select::addOption(Option* option, bool selected)
{
    _options.push_back(option);
    
    Button* pTempButton = new Button(_buttonTexture, ButtonType::NORMAL, true, _scaleFactor, _contextWidth, _contextHeight);
    pTempButton->setLocalAnchor(AnchorPoint::TOP_LEFT);
    pTempButton->setSliced(true);
    pTempButton->setSize(_size.x - 10, _size.y);
    pTempButton->setFont(_font);
    pTempButton->setText(option->text);
    pTempButton->setTextScale(0.25f);
    pTempButton->setPadding(0, -3);
    pTempButton->setColor(0, 0, 0);
    pTempButton->setSecondColor(0, 0, 0);
    _buttonQueue.push_back(pTempButton);
    
    if (selected)
    {
        _selectedId = (unsigned int)_options.size() - 1;
        _pButton->setText(option->text);
    }
}

// Getters
std::string Select::getValue(void)
{
    return _selectedId != -1 ? _options[_selectedId]->value : "";
}

Label* Select::getLabel(void)
{
    return _pButton->getLabel();
}

Button* Select::getButton(void)
{
    return _pButton;
}

View* Select::getView(void)
{
    return _pView;
}

bool Select::isOpen(void)
{
    return _isOpen;
}

// Setters
void Select::setNavigator(Navigator* navigator)
{
    _pView->setNavigator(navigator);
    _pNavigator = navigator;
}

void Select::setSelected(const std::string& select)
{
    for(int i = 0; i < (int)_buttonQueue.size(); i++)
    {
        if (_options[i]->value == select)
        {
            _selectedId = i;
            _pButton->setText(_options[_selectedId]->text);
            _isOpen = false;
            
            updateNavigation();
            break;
        }
    }
}

void Select::setState(const SelectState &state)
{
    _state = state;
    
    if (_state == SelectState::ACTIVATE)
        _pButton->setState(ButtonState::ACTIVATE);
    else
        _pButton->setState(ButtonState::DEFAULT);
}
