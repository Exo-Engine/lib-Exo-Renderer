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

#include <string>
#include <vector>

#include "ISelect.h"
#include "Button.h"
#include "View.h"

namespace   ExoRenderer
{

class Select : public ISelect
{
public:
    Select(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, const std::shared_ptr<Font>& font, float scaleFactor, float contextWidth, float contextHeight);
    ~Select(void);
    
    virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType);
    virtual void updateNavigation(void);
    
    virtual void addOption(Option* option, bool selected = false);
    
    // Getters
    virtual std::string getValue(void);
    virtual Label* getLabel(void);
    virtual Button* getButton(void);
    virtual View* getView(void);
    virtual bool isOpen(void);
    
    // Setters
    virtual void setNavigator(Navigator* navigator);
    virtual void setSelected(const std::string& select);
    virtual void setState(const SelectState &state);
    
    // Override
    virtual void setLocalAnchor(const AnchorPoint& anchor)
    {
        IWidget::setLocalAnchor(anchor);
        _pButton->setLocalAnchor(_localAnchor);
    }
    
    virtual void setPosition(float x, float y)
    {
        IWidget::setPosition(x, y);
        _pButton->setPosition(_position);
    }
    
    virtual void setPosition(const glm::vec2& val)
    {
        IWidget::setPosition(val);
        _pButton->setPosition(_position);
    }
    
    virtual void setVirtualOffset(float x, float y)
    {
        IWidget::setVirtualOffset(x, y);
         _pButton->setVirtualOffset(_virtualOffset);
    }
    
    virtual void setVirtualOffset(const glm::vec2& val)
    {
        IWidget::setVirtualOffset(val);
         _pButton->setVirtualOffset(_virtualOffset);
    }
    
    virtual void setRelativeParentPosition(float x, float y)
    {
        IWidget::setRelativeParentPosition(x, y);
        _pButton->setRelativeParentPosition(_relativeParentPosition);
    }
    
    virtual void setRelativeParentPosition(const glm::vec2& val)
    {
        IWidget::setRelativeParentPosition(val);
        _pButton->setRelativeParentPosition(_relativeParentPosition);
    }
    
    virtual void setSize(float x, float y)
    {
        IWidget::setSize(x, y);
        _pButton->setSize(_size);
    }
    
    virtual void setSize(const glm::vec2& val)
    {
        IWidget::setSize(val);
        _pButton->setSize(_size);
    }
private:
    Navigator               *_pNavigator;
    SelectState             _state;
    
    bool                    _isOpen;
    int                     _selectedId;
    Button*                 _pButton;
    View*                   _pView;
    
    std::vector<Option*>    _options;
    std::vector<Button*>    _buttonQueue;
    
    std::shared_ptr<ITexture> _buttonTexture;
    std::shared_ptr<Font> _font;
};

}
