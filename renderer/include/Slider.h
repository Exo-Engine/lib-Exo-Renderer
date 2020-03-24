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

#pragma once

#include <glm/vec2.hpp>

#include "ISlider.h"
#include "Image.h"
#include "Button.h"

#define SLIDER_BUTTON_WIDTH 5

namespace   ExoRenderer
{

class Slider : public ISlider
{
public:
    Slider(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& barTexture, float scaleFactor, float contextWidth, float contextHeight);
    ~Slider(void);

    virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType);
    
    // Getters
    virtual float getValue(void) const;
    
    virtual Button* getSliderButton(void);
    virtual Image* getBarImage(void);
    
    // Setters
    virtual void setValue(float value);
    virtual void setSelected(bool selected);
    
    // Override
    virtual void setLocalAnchor(const AnchorPoint& anchor)
    {
        IWidget::setLocalAnchor(anchor);
        _pBar->setLocalAnchor(_localAnchor);
    }
    
    virtual void setPosition(float x, float y)
    {
        IWidget::setPosition(x, y);
        _pBar->setPosition(getUnscaleRealPosition().x - _size.x + (((_size.x * 2) / 100) * _value), getUnscaleRealPosition().y);
        _pButton->setPosition(_position);
    }
    
    virtual void setPosition(const glm::vec2& val)
    {
        IWidget::setPosition(val);
        _pBar->setPosition(getUnscaleRealPosition().x - _size.x + (((_size.x * 2) / 100) * _value), getUnscaleRealPosition().y);
        _pButton->setPosition(_position);
    }
    
    virtual void setVirtualOffset(float x, float y)
    {
        IWidget::setVirtualOffset(x, y);
        _pBar->setVirtualOffset(_virtualOffset);
        _pButton->setVirtualOffset(_virtualOffset);
    }
    
    virtual void setVirtualOffset(const glm::vec2& val)
    {
        IWidget::setVirtualOffset(val);
        _pBar->setVirtualOffset(_virtualOffset);
        _pButton->setVirtualOffset(_virtualOffset);
    }
    
    virtual void setRelativeParentPosition(float x, float y)
    {
        IWidget::setRelativeParentPosition(x, y);
        _pBar->setRelativeParentPosition(_relativeParentPosition);
        _pButton->setRelativeParentPosition(_relativeParentPosition);
    }
    
    virtual void setRelativeParentPosition(const glm::vec2& val)
    {
        IWidget::setRelativeParentPosition(val);
        _pBar->setRelativeParentPosition(_relativeParentPosition);
        _pButton->setRelativeParentPosition(_relativeParentPosition);
    }
    
    virtual void setSize(float x, float y)
    {
        IWidget::setSize(x, y);
        _pBar->setSize(_size);
        _pButton->setSize(SLIDER_BUTTON_WIDTH, _size.y + 4);
    }
    
    virtual void setSize(const glm::vec2& val)
    {
        IWidget::setSize(val);
        _pBar->setSize(_size);
        _pButton->setSize(SLIDER_BUTTON_WIDTH, _size.y + 4);
    }
private:
    void updatePosition(void);
private:
    float _value;
    Image*  _pBar;
    Button* _pButton;
    
    bool _selected;
};

}
