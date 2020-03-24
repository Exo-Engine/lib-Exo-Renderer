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

#include "IGamepadManager.h"
#include "Gamepad.h"

#include <vector>

namespace   ExoRendererSDLOpenGL
{

class GamepadManager : public ExoRenderer::IGamepadManager
{
public:
    GamepadManager(void);
    virtual ~GamepadManager(void);
    
    virtual void add(int32_t id);
    virtual void remove(int32_t id);
    
    void update(void);
    
    void keyDown(int32_t gamepadId, const ExoRenderer::GamepadButtons &id);
    void keyUp(int32_t gamepadId, const ExoRenderer::GamepadButtons &id);
    
    void leftStickX(int32_t gamepadId, float x);
    void leftStickY(int32_t gamepadId, float y);
    
    void rightStickX(int32_t gamepadId, float x);
    void rightStickY(int32_t gamepadId, float y);
    
    void triggerLeft(int32_t gamepadId, float value);
    void triggerRight(int32_t gamepadId, float value);
    
    // Getters
    virtual unsigned int getGamepadNumber(void);
    virtual ExoRenderer::IGamepad *getGamepad(unsigned int id);
private:
    Gamepad*              _fakeGamepad; // Always with default data, if gamepad list is empty
    std::vector<Gamepad*> _gamepadList;
};

}
