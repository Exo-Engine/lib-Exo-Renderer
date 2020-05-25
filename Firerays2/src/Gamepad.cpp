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

#include <SDL2/SDL_gamecontroller.h>
#include <iostream>
#include "Gamepad.h"

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

Gamepad::Gamepad(Sint32 id)
: _pGameController(nullptr)
{
	_pGameController = SDL_GameControllerOpen(id);
}

Gamepad::~Gamepad(void)
{
	if (_pGameController != NULL)
	{
		if (SDL_WasInit(SDL_INIT_EVERYTHING))
			SDL_GameControllerClose(_pGameController);
		_pGameController = nullptr;
	}
}

void Gamepad::keyDown(const GamepadButtons &id)
{
	if (id < (GamepadButtons)GAMEPAD_BUTTON_MAX)
		_buffer[id] = true;
}

void Gamepad::keyUp(const GamepadButtons &id)
{
	if (id < (GamepadButtons)GAMEPAD_BUTTON_MAX)
		_buffer[id] = false;
}

bool Gamepad::isKeyDown(const GamepadButtons &id) const
{
	if (id < (GamepadButtons)GAMEPAD_BUTTON_MAX)
		return _buffer[id];
	else
		return false;
}

bool Gamepad::lastIsKeyDown(const GamepadButtons &id) const
{
	if (id < (GamepadButtons)GAMEPAD_BUTTON_MAX)
		return _lastBuffer[id];
	else
		return false;
}

// Getters
SDL_GameController *Gamepad::getController(void)
{
	return _pGameController;
}

GamepadButtons Gamepad::getGamepadInput(const unsigned int &id)
{
	switch (id) {
		case SDL_CONTROLLER_BUTTON_A:				return GamepadButtons::BUTTON_A;
		case SDL_CONTROLLER_BUTTON_B:				return GamepadButtons::BUTTON_B;
		case SDL_CONTROLLER_BUTTON_X:				return GamepadButtons::BUTTON_X;
		case SDL_CONTROLLER_BUTTON_Y:				return GamepadButtons::BUTTON_Y;
		case SDL_CONTROLLER_BUTTON_BACK:			return GamepadButtons::BUTTON_BACK;
		case SDL_CONTROLLER_BUTTON_GUIDE:			return GamepadButtons::BUTTON_GUIDE;
		case SDL_CONTROLLER_BUTTON_START:			return GamepadButtons::BUTTON_START;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK:		return GamepadButtons::BUTTON_LEFTSTICK;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:		return GamepadButtons::BUTTON_RIGHTSTICK;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:	return GamepadButtons::BUTTON_LEFTSHOULDER;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:	return GamepadButtons::BUTTON_RIGHTSHOULDER;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:		 return GamepadButtons::BUTTON_DPAD_UP;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:		return GamepadButtons::BUTTON_DPAD_DOWN;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:		return GamepadButtons::BUTTON_DPAD_LEFT;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:		return GamepadButtons::BUTTON_DPAD_RIGHT;
		case SDL_CONTROLLER_BUTTON_MAX:			 return GamepadButtons::GAMEPAD_BUTTON_MAX;
		default:									return GamepadButtons::GAMEPAD_BUTTON_UNKNOW;
	}
}
