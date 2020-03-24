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

#include "IKeyboard.h"
#include "IGamepad.h"

#include "IWidget.h"
#include "IButton.h"
#include "ICheckbox.h"
#include "IView.h"
#include "IInput.h"
#include "ISelect.h"
#include "ISlider.h"

namespace   ExoRenderer
{

class Navigator
{
public:
    Navigator(void) : _pSelectedWidget(nullptr), _lastSelectedDeltaTime(0.0f) {}
    virtual ~Navigator(void) {}
    
    void init(void)
    {
        _pSelectedWidget = nullptr;
        _lastSelectedDeltaTime = 0.0f;
    }
    
    void update(double delta, IGamepad* gamepad, const NavigationType& navigationType)
    {
        // Update selected widget - Game Controller
        if (gamepad->IsGamepad() && navigationType == NavigationType::GAMEPAD)
        {
            _lastSelectedDeltaTime += delta;
            if (_pSelectedWidget && _lastSelectedDeltaTime > 200.0f) // 0.2 second
            {
                if (_pSelectedWidget->getNextWidget(WidgetNext::BOTTOM) && gamepad->leftStick.y > 0 + GAMEPAD_DEAD_ZONE)
                    updateSelectedWidget(_pSelectedWidget->getNextWidget(WidgetNext::BOTTOM), gamepad, navigationType);
                else if (_pSelectedWidget->getNextWidget(WidgetNext::TOP) && gamepad->leftStick.y < 0 - GAMEPAD_DEAD_ZONE)
                    updateSelectedWidget(_pSelectedWidget->getNextWidget(WidgetNext::TOP), gamepad, navigationType);
                else if (_pSelectedWidget->getNextWidget(WidgetNext::RIGHT) && gamepad->leftStick.x > 0 + GAMEPAD_DEAD_ZONE)
                    updateSelectedWidget(_pSelectedWidget->getNextWidget(WidgetNext::RIGHT), gamepad, navigationType);
                else if (_pSelectedWidget->getNextWidget(WidgetNext::LEFT) && gamepad->leftStick.x < 0 - GAMEPAD_DEAD_ZONE)
                    updateSelectedWidget(_pSelectedWidget->getNextWidget(WidgetNext::LEFT), gamepad, navigationType);
                
                if (gamepad->isKeyDown(GamepadButtons::BUTTON_A) && gamepad->lastIsKeyDown(GamepadButtons::BUTTON_A))
                {
                    switch (_pSelectedWidget->getType())
                    {
                        case IWidget::BUTTON: {
                            if (_pSelectedWidget)
                                ((IButton*)_pSelectedWidget)->setState(ButtonState::SELECTED);
                            break;
                        }
                        default: break;
                    }
                }
            }
        }
        else if (_lastNavigationType != navigationType)
        {
            switch (_pSelectedWidget->getType())
            {
                case IWidget::BUTTON: {
                    ((IButton*)_pSelectedWidget)->setState(ButtonState::DEFAULT);
                    break;
                }
                case IWidget::CHECKBOX: {
                    if (_pSelectedWidget)
                        ((ICheckbox*)_pSelectedWidget)->setSelected(false);
                    break;
                }
                case IWidget::INPUT: {
                    ((IInput*)_pSelectedWidget)->setSelected(false);
                    break;
                }
                case IWidget::SLIDER: {
                    ((ISlider*)_pSelectedWidget)->setSelected(false);
                    break;
                }
                case IWidget::SELECT: {
                    ((ISelect*)_pSelectedWidget)->setState(SelectState::DEFAULT);
                    break;
                }
                default: break;
            }
        }
        
        _lastNavigationType = navigationType;
    }
    
    void updateSelectedWidget(IWidget* widget, IGamepad* gamepad, const NavigationType& navigationType)
    {
        // Unselect current
        if (_pSelectedWidget)
        {
            switch (_pSelectedWidget->getType())
            {
                case IWidget::BUTTON: {
                    ((IButton*)_pSelectedWidget)->setState(ButtonState::DEFAULT);
                    break;
                }
                case IWidget::CHECKBOX: {
                    ((ICheckbox*)_pSelectedWidget)->setSelected(false);
                    break;
                }
                case IWidget::INPUT: {
                    ((IInput*)_pSelectedWidget)->setSelected(false);
                    break;
                }
                case IWidget::SELECT: {
                    ((ISelect*)_pSelectedWidget)->setState(SelectState::DEFAULT);
                    break;
                }
                case IWidget::SLIDER: {
                    ((ISlider*)_pSelectedWidget)->setSelected(false);
                    break;
                }
                default: break;
            }
        }
            
        // Update current
        if (widget)
        {
            _pSelectedWidget = widget;
            if (gamepad->IsGamepad() && navigationType == NavigationType::GAMEPAD)
                switch (widget->getType())
                {
                    case IWidget::BUTTON: {
                        ((IButton*)_pSelectedWidget)->setState(ButtonState::ACTIVATE);
                        break;
                    }
                    case IWidget::CHECKBOX: {
                        if (_pSelectedWidget)
                            ((ICheckbox*)_pSelectedWidget)->setSelected(true);
                        break;
                    }
                    case IWidget::INPUT: {
                        ((IInput*)_pSelectedWidget)->setSelected(true);
                        break;
                    }
                    case IWidget::SLIDER: {
                        ((ISlider*)_pSelectedWidget)->setSelected(true);
                        break;
                    }
                    case IWidget::SELECT: {
                        ((ISelect*)_pSelectedWidget)->setState(SelectState::ACTIVATE);
                        break;
                    }
                    case IWidget::VIEW: {
                        ((IView*)_pSelectedWidget)->setState(ViewState::SELECTED);
                        if (_pSelectedWidget->getNextWidget(WidgetNext::TOP))
                            updateSelectedWidget(_pSelectedWidget->getNextWidget(WidgetNext::TOP), gamepad, navigationType);
                        break;
                    }
                    default: break;
                }
        }
        
        // Reset
        _lastSelectedDeltaTime = 0.0f;
    }
private:
    NavigationType   _lastNavigationType;
    IWidget*         _pSelectedWidget;
    float            _lastSelectedDeltaTime;
};

}
