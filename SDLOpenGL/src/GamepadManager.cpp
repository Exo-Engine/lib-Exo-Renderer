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

#include "GamepadManager.h"

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

GamepadManager::GamepadManager(void)
{
	_fakeGamepad = new Gamepad(-1);
	_fakeGamepad->setIsGamepad(false);
}

GamepadManager::~GamepadManager(void)
{
	if (_fakeGamepad)
		delete _fakeGamepad;

	while (!_gamepadList.empty())
	{
		delete _gamepadList.back();
		_gamepadList.pop_back();
	}
	_gamepadList.clear();
}


void GamepadManager::add(int32_t id)
{
	if (SDL_IsGameController(id))
		_gamepadList.push_back(new Gamepad(id));
}

void GamepadManager::remove(int32_t id)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(id))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == id)
			{
				delete _gamepadList[i];
				_gamepadList.erase(_gamepadList.begin() + i);
				break;
			}
		}
}

void GamepadManager::update(void)
{
	for (auto& gamepad : _gamepadList)
		gamepad->updateLastBuffer();
}

void GamepadManager::keyDown(int32_t gamepadId, const GamepadButtons &id)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(gamepadId))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == gamepadId)
			{
				_gamepadList[i]->keyDown(id);
				break;
			}
		}
}

void GamepadManager::keyUp(int32_t gamepadId, const GamepadButtons &id)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(gamepadId))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == gamepadId)
			{
				_gamepadList[i]->keyUp(id);
				break;
			}
		}
}

void GamepadManager::leftStickX(int32_t gamepadId, float x)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(gamepadId))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == gamepadId)
			{
				_gamepadList[i]->leftStick.x = x;
				break;
			}
		}
}

void GamepadManager::leftStickY(int32_t gamepadId, float y)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(gamepadId))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == gamepadId)
			{
				_gamepadList[i]->leftStick.y = y;
				break;
			}
		}
}

void GamepadManager::rightStickX(int32_t gamepadId, float x)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(gamepadId))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == gamepadId)
			{
				_gamepadList[i]->rightStick.x = x;
				break;
			}
		}
}

void GamepadManager::rightStickY(int32_t gamepadId, float y)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(gamepadId))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == gamepadId)
			{
				_gamepadList[i]->rightStick.y = y;
				break;
			}
		}
}

void GamepadManager::triggerLeft(int32_t gamepadId, float value)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(gamepadId))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == gamepadId)
			{
				_gamepadList[i]->triggerLeft = value;
				break;
			}
		}
}

void GamepadManager::triggerRight(int32_t gamepadId, float value)
{
	for (int i = 0; i < _gamepadList.size(); i++)
		if (SDL_IsGameController(gamepadId))
		{
			if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(_gamepadList[i]->getController())) == gamepadId)
			{
				_gamepadList[i]->triggerRight = value;
				break;
			}
		}
}

// Getters
unsigned int GamepadManager::getGamepadNumber(void)
{
	return (unsigned int)_gamepadList.size();
}

IGamepad *GamepadManager::getGamepad(unsigned int id)
{
	if (id < _gamepadList.size())
		return _gamepadList[id];
	else
		return _fakeGamepad;
}
