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

#include "Camera.h"
#include "RendererSDLOpenGL.h"

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Camera::Camera(void)
: ICamera(), _lookAt(0.0f)
{
	
}

Camera::~Camera(void)
{
	
}

void Camera::update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad)
{
	(void)mouse;
	
	// Keyboard & Gamepad
	if (keyboard->isKeyDown(KeyboardKeys::KEY_SPACE) || gamepad->triggerLeft > 0)
		_position.z += _speed * RendererSDLOpenGL::Get().getWindow()->getDelta();
	else if ((keyboard->isKeyDown(KeyboardKeys::KEY_LSHIFT) &&	_position.z > 1.0f) || (gamepad->triggerRight > 0 &&	_position.z > 3.0f))
		_position.z -= _speed * RendererSDLOpenGL::Get().getWindow()->getDelta();
	
	if (_pFollowedEntity == nullptr)
	{
		if (keyboard->isKeyDown(KeyboardKeys::KEY_W) || gamepad->leftStick.y < 0 - GAMEPAD_DEAD_ZONE)
			_position.y += _speed * RendererSDLOpenGL::Get().getWindow()->getDelta();
		else if (keyboard->isKeyDown(KeyboardKeys::KEY_S) || gamepad->leftStick.y > 0 + GAMEPAD_DEAD_ZONE)
			_position.y -= _speed * RendererSDLOpenGL::Get().getWindow()->getDelta();
		
		if (keyboard->isKeyDown(KeyboardKeys::KEY_A) || gamepad->leftStick.x < 0 - GAMEPAD_DEAD_ZONE)
			_position.x -= _speed * RendererSDLOpenGL::Get().getWindow()->getDelta();
		else if (keyboard->isKeyDown(KeyboardKeys::KEY_D) || gamepad->leftStick.x > 0 + GAMEPAD_DEAD_ZONE)
			_position.x += _speed * RendererSDLOpenGL::Get().getWindow()->getDelta();
	}

	// Calculate LookAt matrice
	if (_pFollowedEntity == nullptr)
		_lookAt = glm::lookAt(_position, glm::vec3(_position.x, _position.y, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	else
		_lookAt = glm::lookAt(glm::vec3(_pFollowedEntity->x + _pFollowedEntitySize->x / 2, _pFollowedEntity->y + _pFollowedEntitySize->y / 2, _position.z), glm::vec3(_pFollowedEntity->x + _pFollowedEntitySize->x / 2, _pFollowedEntity->y + _pFollowedEntitySize->y / 2, 0), glm::vec3(0.0f, 1.0f, 0.0f));
}

// Getters
glm::mat4 Camera::getLookAt(void)
{
	return _lookAt;
}
