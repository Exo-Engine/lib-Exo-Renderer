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

#include "IWidget.h"
#include <iostream>

using namespace ExoRenderer;

// Class Widget
IWidget::IWidget(const WidgetType& type)
:   _type(type), _enabled(true), _anchor(AnchorPoint::TOP_LEFT), _localAnchor(AnchorPoint::CENTER),
    _position(glm::vec2(0.0f, 0.0f)), _virtualOffset(glm::vec2(0.0f, 0.0f)), _relativeParentPosition(0.0f, 0.0f),
    _size(glm::vec2(1.0f, 1.0f)), _opacity(0.0f), _scaleFactor(0), _contextWidth(0), _contextHeight(0), _scale(true),
    _scalePosition(glm::vec2(0.0f, 0.0f)), _scaleSize(glm::vec2(1.0f, 1.0f)),
    _callback(nullptr), _callbackFunction(nullptr)
{
    _pNextWidget[0] = nullptr;
    _pNextWidget[1] = nullptr;
    _pNextWidget[2] = nullptr;
    _pNextWidget[3] = nullptr;
}

IWidget::~IWidget(void)
{   }
