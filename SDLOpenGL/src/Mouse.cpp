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

#include <SDL2/SDL_mouse.h>
#include "Mouse.h"

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Mouse::Mouse(void)
{   }

Mouse::~Mouse(void)
{   }

void Mouse::keyDown(const MouseButtons &id)
{
    if (id < (MouseButtons)MOUSE_BUTTON_MAX)
        _buffer[id] = true;
}

void Mouse::keyUp(const MouseButtons &id)
{
    if (id < (MouseButtons)MOUSE_BUTTON_MAX)
        _buffer[id] = false;
}

bool Mouse::isKeyDown(const MouseButtons &id) const
{
    if (id < (MouseButtons)MOUSE_BUTTON_MAX)
        return _buffer[id];
    else
        return false;
}

bool Mouse::lastIsKeyDown(const MouseButtons &id) const
{
    if (id < (MouseButtons)MOUSE_BUTTON_MAX)
        return _lastBuffer[id];
    else
        return false;
}

// Getters
MouseButtons Mouse::getMouseInput(const unsigned int &id) const
{
    switch (id) {
        case SDL_BUTTON_LEFT:       return MouseButtons::BUTTON_LEFT;
        case SDL_BUTTON_RIGHT:      return MouseButtons::BUTTON_RIGHT;
        case SDL_BUTTON_MIDDLE:     return MouseButtons::BUTTON_MIDDLE;
        default:                    return MouseButtons::MOUSE_BUTTON_UNKNOW;
    }
}
