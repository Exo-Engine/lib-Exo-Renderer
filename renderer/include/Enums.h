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

#define REFRENCE_RESOLUTION_WIDTH 1280
#define REFRENCE_RESOLUTION_HEIGHT 720

namespace	ExoRenderer
{

enum NavigationType
{
	MOUSE,
	GAMEPAD
};

// Buffer
enum BufferType
{
	VERTEXARRAY,
	ARRAYBUFFER,
	INDEXBUFFER,
	RENDERBUFFER,
};

enum BufferDraw
{
	DYNAMIC,
	STATIC
};

// Texture
enum TextureFilter
{
	NEAREST,
	LINEAR,
};

enum TextureFormat
{
	RGB,
	RGBA,
	DEPTH
};

// Window
enum WindowMode
{
	WINDOWED = 0,
	FULLSCREEN,
	BORDERLESS
};

// Inputs
enum MouseButtons
{
	BUTTON_LEFT,
	BUTTON_MIDDLE,
	BUTTON_RIGHT,
	MOUSE_BUTTON_MAX,
	MOUSE_BUTTON_UNKNOW
};

enum GamepadButtons
{
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_BACK,
	BUTTON_GUIDE,
	BUTTON_START,
	BUTTON_LEFTSTICK,
	BUTTON_RIGHTSTICK,
	BUTTON_LEFTSHOULDER,
	BUTTON_RIGHTSHOULDER,
	BUTTON_DPAD_UP,
	BUTTON_DPAD_DOWN,
	BUTTON_DPAD_LEFT,
	BUTTON_DPAD_RIGHT,
	GAMEPAD_BUTTON_MAX,
	GAMEPAD_BUTTON_UNKNOW
};

enum KeyboardKeys
{
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_KP_0,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,
	KEY_A,
	KEY_B,
	KEY_BACKSLASH,
	KEY_BACKSPACE,
	KEY_C,
	KEY_CAPSLOCK,
	KEY_D,
	KEY_DELETE,
	KEY_DOWN,
	KEY_E,
	KEY_ESCAPE,
	KEY_F,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_LALT,
	KEY_LCTRL,
	KEY_LEFT,
	KEY_LSHIFT,
	KEY_M,
	KEY_N,
	KEY_NUMLOCKCLEAR,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_RIGHT,
	KEY_RALT,
	KEY_RCTRL,
	KEY_RETURN,
	KEY_RSHIFT,
	KEY_S,
	KEY_SPACE,
	KEY_T,
	KEY_TAB,
	KEY_U,
	KEY_UP,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PERIOD,
	KEY_COMMA,
	KEY_MINUS,
	KEY_KP_MINUS,
	KEY_MAX,
	KEY_UNKNOW
};

}
