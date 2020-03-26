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

#pragma once

#include <SDL2/SDL.h>
#include "IGamepad.h"

namespace	ExoRendererSDLOpenGL
{

class Gamepad : public ExoRenderer::IGamepad
{
public:
	Gamepad(Sint32 id);
	virtual ~Gamepad(void);

	virtual void keyDown(const ExoRenderer::GamepadButtons &id);
	virtual void keyUp(const ExoRenderer::GamepadButtons &id);
	
	virtual bool isKeyDown(const ExoRenderer::GamepadButtons &id) const;
	virtual bool lastIsKeyDown(const ExoRenderer::GamepadButtons &id) const;

	// Getters
	SDL_GameController *getController(void);
	static ExoRenderer::GamepadButtons getGamepadInput(const unsigned int &id);
private:
	SDL_GameController *_pGameController;
};

}
