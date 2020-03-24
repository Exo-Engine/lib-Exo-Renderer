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

#include "View.h"
#include "Input.h"
#include "Select.h"
#include "Navigator.h"

using namespace ExoRenderer;

View::View(const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows, unsigned int numberOfColumns, float scaleFactor, float contextWidth, float contextHeight)
: IView(), _pNavigator(nullptr), _autoScroll(false), _mouseOut(0), _firstShow(-1), _scrollY(0.0f), _scrollHeight(0.0f), _lastScrollHeight(_scrollHeight), _parentScissor(0.0f, 0.0f, 0.0f, 0.0f), _scrollbarButton(nullptr)
{
    _scaleFactor = scaleFactor;
    _contextWidth = contextWidth;
    _contextHeight = contextHeight;
    
    initialize(scrollTexture, numberOfRows, numberOfColumns);
}

View::View(const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows, unsigned int numberOfColumns, float scaleFactor, float contextWidth, float contextHeight)
: IView(), _autoScroll(false), _firstShow(-1), _scrollY(0.0f), _scrollHeight(0.0f), _lastScrollHeight(_scrollHeight), _parentScissor(0.0f, 0.0f, 0.0f, 0.0f), _backgroundTexture(backgroundTexture), _scrollbarButton(nullptr)
{
    _scaleFactor = scaleFactor;
    _contextWidth = contextWidth;
    _contextHeight = contextHeight;
    
    initialize(scrollTexture, numberOfRows, numberOfColumns);
}

View::~View(void)
{
    if (_scrollbarButton)
        delete _scrollbarButton;
}

void View::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType)
{
    // Calculate scrollbar position
    if (_firstShow == 0 || _scrollHeight != _lastScrollHeight)
    {
        if (_scrollHeight > getRealPosition().y + _relativeParentPosition.y + getScaleSize().y)
        {
            _scrollbarButton->setRelativeParentPosition(_relativeParentPosition);
            _scrollbarButton->setSize(6.0f, (getScaleSize().y * ((getRealPosition().y + _relativeParentPosition.y + getScaleSize().y) / _scrollHeight)) / _scaleFactor);
            if (_firstShow == 0)
                _scrollbarButton->setPosition(getUnscaleRealPosition().x + _size.x, getUnscaleRealPosition().y - _size.y + _scrollbarButton->getSize().y);
            else
                _scrollbarButton->setPosition(getUnscaleRealPosition().x + _size.x, _scrollbarButton->getPosition().y);
            
            if (_firstShow != 0)
                calculateScrollY(_autoScroll ? _realPosition.y + _scaleSize.y + 1 : _scrollbarButton->getScalePosition().y);
        }
        _lastScrollHeight = _scrollHeight;
        _firstShow = 1;
    }
    else if (_firstShow == -1)
        _firstShow = 0;
    
    _scrollbarButton->setVirtualOffset(_virtualOffset);
    
    // Wheel or Gamepad
    if (_scrollHeight > getRealPosition().y + _relativeParentPosition.y + getScaleSize().y)
    {
        if (mouse->wheelY != 0 && _realPosition.y + _virtualOffset.y + _relativeParentPosition.y + _scaleSize.y > mouse->y // Y
            && _realPosition.y + _virtualOffset.y + _relativeParentPosition.y - _scaleSize.y < mouse->y
            && _realPosition.x + _virtualOffset.x + _relativeParentPosition.x + _scaleSize.x > mouse->x // X
            && _realPosition.x + _virtualOffset.x + _relativeParentPosition.x - _scaleSize.x < mouse->x)
        {
            calculateScrollY(_scrollbarButton->getScalePosition().y + mouse->wheelY);
        }
    }
    
    if (_state == ViewState::SELECTED)
    {
        if (_pNavigator
            && getNextWidget(WidgetNext::LEFT)
            && gamepad->isKeyDown(GamepadButtons::BUTTON_B)
            && gamepad->lastIsKeyDown(GamepadButtons::BUTTON_B))
        {
            _pNavigator->updateSelectedWidget(getNextWidget(WidgetNext::LEFT), gamepad, navigationType);
            _state = ViewState::DEFAULT;
        }
        
        if (_scrollHeight > getRealPosition().y + _relativeParentPosition.y + getScaleSize().y)
        {
            if (gamepad->rightStick.y > 0 + GAMEPAD_DEAD_ZONE)
                calculateScrollY(_scrollbarButton->getScalePosition().y + 2);
            else if (gamepad->rightStick.y < 0 - GAMEPAD_DEAD_ZONE)
                calculateScrollY(_scrollbarButton->getScalePosition().y - 2);
        }
    }
    
    // Mouse scroll button
    _scrollbarButton->update(mouse, keyboard, gamepad, navigationType);
    if (_scrollbarButton->getClickEvent())
        calculateScrollY(mouse->y - _relativeParentPosition.y);
    
    if (!(!(_realPosition.y + _virtualOffset.y + _relativeParentPosition.y + _scaleSize.y > mouse->y // Y
            && _realPosition.y + _virtualOffset.y + _relativeParentPosition.y - _scaleSize.y < mouse->y
            && _realPosition.x + _virtualOffset.x + _relativeParentPosition.x + _scaleSize.x > mouse->x // X
            && _realPosition.x + _virtualOffset.x + _relativeParentPosition.x - _scaleSize.x < mouse->x)) || _state == ViewState::SELECTED)
    {
        for(auto& widget : _renderQueue)
            widget->update(mouse, keyboard, gamepad, navigationType);
        
        _mouseOut = 0;
    }
    else if (_mouseOut == 0)
        _mouseOut = 1;
    
    if (_mouseOut == 1)
    {
        for (auto& widget : _renderQueue)
            if (widget->getType() == WidgetType::BUTTON)
                ((Button*)widget)->resetDefaultState();
        _mouseOut = 2;
    }
    
    _scrollHeight = 0.0f;
}

void View::addChild(IWidget *widget)
{
    // Set element position
    widget->setRelativeParentPosition(_relativeParentPosition + getRealPosition() - _scaleSize);
    widget->setVirtualOffset(0, _scrollY + _virtualOffset.y);

    float position = widget->getRelativeParentPosition().y + widget->getRealPosition().y + widget->getScaleSize().y;
    if (position > _scrollHeight)
        _scrollHeight = position;
    
    // Add the element in the view
    if (widget->getRealPosition().y + widget->getVirtualOffset().y + widget->getRelativeParentPosition().y - widget->getScaleSize().y < getRealPosition().y + _relativeParentPosition.y + getScaleSize().y // Bottom
        && widget->getRealPosition().y + widget->getVirtualOffset().y + widget->getRelativeParentPosition().y + widget->getScaleSize().y > getRealPosition().y + _relativeParentPosition.y - getScaleSize().y) // Top
    {
        widget->setIsEnabled(true);
        pushWidget(widget);
        _renderQueue.push_back(widget);
    }
    else
        widget->setIsEnabled(false);
}

void View::addChild(ILabel *label)
{
    label->setRelativeParentPosition(_realPosition + _relativeParentPosition - _scaleSize);
    label->setVirtualOffset(0, _scrollY + _virtualOffset.y);
    label->contextInfo(_scaleFactor, _contextWidth, _contextHeight, true);
    
    float position = label->getRelativeParentPosition().y + label->getRealPosition().y + label->getGlyphLayout().y;
    if (position > _scrollHeight)
        _scrollHeight = position;
    
    // Add the element in the view
    if (label->getRealPosition().y + label->getVirtualOffset().y + label->getRelativeParentPosition().y - label->getGlyphLayout().y < getRealPosition().y + _relativeParentPosition.y + getScaleSize().y // Bottom
        && label->getRealPosition().y + label->getVirtualOffset().y + label->getRelativeParentPosition().y + label->getGlyphLayout().y > getRealPosition().y + _relativeParentPosition.y - getScaleSize().y) // Top
    {
        _labelRenderQueue.push_back((Label*)label);
    }
}

// Getters
const ViewState &View::getState(void)
{
    return _state;
}

const std::shared_ptr<ITexture>& View::getBackgroundTexture(void)
{
    return _backgroundTexture;
}

float View::getScrollHeight(void)
{
    return _scrollHeight;
}

float View::getLastScrollHeight(void)
{
    return _lastScrollHeight;
}

Button* View::getScrollbarButton(void)
{
    return _scrollbarButton;
}

glm::vec4& View::getParentScissor(void)
{
    return _parentScissor;
}

std::deque<IWidget*>& View::getRenderQueue(void)
{
    return _renderQueue;
}

std::deque<IWidget*>& View::getFrontRenderQueue(void)
{
    return _frontRenderQueue;
}

std::deque<Label*>& View::getLabelRenderQueue(void)
{
    return _labelRenderQueue;
}

// Setters
void View::setState(const ViewState &state)
{
    _state = state;
}

void View::setNavigator(Navigator* navigator)
{
    _pNavigator = navigator;
}

void View::setParentScissor(int x, int y, int w, int h)
{
    _parentScissor = glm::vec4(x, y, w, h);
}

void View::setAutoScroll(bool autoScroll)
{
    _autoScroll = autoScroll;
}

// Private
void View::initialize(const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows, unsigned int numberOfColumns)
{
    // Calculate position with the anchor
    _scrollbarButton = new Button(scrollTexture, ButtonType::DRAGGABLE, false, _scaleFactor, _contextWidth, _contextHeight);
    _scrollbarButton->setSliced(true);
}

void View::pushWidget(IWidget* widget)
{
    switch (widget->getType())
    {
        case BUTTON: {
            auto button = (Button*)widget;
            button->updateLabel();
            _labelRenderQueue.push_back(button->getLabel());
            break;
        }
        case SELECT: {
            auto select = (Select*)widget;
            select->getButton()->updateLabel();
            _labelRenderQueue.push_back(select->getLabel());
            
            if (select->isOpen())
            {
                select->getView()->setParentScissor(_realPosition.x - _scaleSize.x, (_realPosition.y + _scaleSize.y - _contextHeight) * -1 - _virtualOffset.y, _scaleSize.x * 2, _scaleSize.y * 2);
                _frontRenderQueue.push_back(widget);
            }
            break;
        }
        case INPUT: {
            auto input = (Input*)widget;
            input->updateLabel();
            _labelRenderQueue.push_back(input->getLabel());
            break;
        }
        case VIEW: {
            auto area = (View*)widget;
            area->setAnchor(AnchorPoint::TOP_LEFT);
            area->setParentScissor(_realPosition.x - _scaleSize.x, (_realPosition.y + _scaleSize.y - _contextHeight) * -1 - _virtualOffset.y, _scaleSize.x * 2, _scaleSize.y * 2);
            break;
        }
        default: break;
    }
}

void View::calculateScrollY(float yPos)
{
    // Scrollbar position
    if (yPos - _scrollbarButton->getScaleSize().y < _realPosition.y - _scaleSize.y)
        yPos = _realPosition.y - _scaleSize.y + _scrollbarButton->getScaleSize().y;
    else if (yPos + _scrollbarButton->getScaleSize().y > _realPosition.y + _scaleSize.y)
        yPos = _realPosition.y + _scaleSize.y - _scrollbarButton->getScaleSize().y;
    
    _scrollbarButton->setPosition(_scrollbarButton->getPosition().x, yPos / _scaleFactor);
    
    // Virtual scroll
    _scrollY = -(yPos - (_realPosition.y - _scaleSize.y + _scrollbarButton->getScaleSize().y)) / (_scaleSize.y * 2 / _scrollHeight);
}
