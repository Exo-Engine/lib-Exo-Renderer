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

#include "Keyboard.h"
#include <SDL2/SDL_keyboard.h>

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Keyboard::Keyboard()
{   }

Keyboard::~Keyboard()
{   }

void Keyboard::keyDown(const KeyboardKeys &id)
{
    if (id < KEY_MAX)
        _buffer[id] = true;
}

void Keyboard::keyUp(const KeyboardKeys &id)
{
    if (id < KEY_MAX)
        _buffer[id] = false;
}

bool Keyboard::isKeyDown(const KeyboardKeys &id) const
{
    if (id < KEY_MAX)
        return _buffer[id];
    else
        return false;
}

bool Keyboard::lastIsKeyDown(const KeyboardKeys &id) const
{
    if (id < KEY_MAX)
        return _lastBuffer[id];
    else
        return false;
}

const char* Keyboard::convertKeyboardKeyToChar(const KeyboardKeys &key) const
{
    switch (key)
    {
        case KEY_0:case KEY_KP_0:           return "0";
        case KEY_1:case KEY_KP_1:           return "1";
        case KEY_2:case KEY_KP_2:           return "2";
        case KEY_3:case KEY_KP_3:           return "3";
        case KEY_4:case KEY_KP_4:           return "4";
        case KEY_5:case KEY_KP_5:           return "5";
        case KEY_6:case KEY_KP_6:           return "6";
        case KEY_7:case KEY_KP_7:           return "7";
        case KEY_8:case KEY_KP_8:           return "8";
        case KEY_9:case KEY_KP_9:           return "9";
        case KEY_A:                         return "A";
        case KEY_B:                         return "B";
        case KEY_BACKSLASH:                 return "\\";
        case KEY_C:                         return "C";
        case KEY_D:                         return "D";
        case KEY_E:                         return "E";
        case KEY_F:                         return "F";
        case KEY_G:                         return "G";
        case KEY_H:                         return "H";
        case KEY_I:                         return "I";
        case KEY_J:                         return "J";
        case KEY_K:                         return "K";
        case KEY_L:                         return "L";
        case KEY_M:                         return "M";
        case KEY_N:                         return "N";
        case KEY_O:                         return "O";
        case KEY_P:                         return "P";
        case KEY_Q:                         return "Q";
        case KEY_R:                         return "R";
        case KEY_S:                         return "S";
        case KEY_SPACE:                     return " ";
        case KEY_T:                         return "T";
        case KEY_TAB:                       return "\t";
        case KEY_U:                         return "U";
        case KEY_V:                         return "V";
        case KEY_W:                         return "W";
        case KEY_X:                         return "X";
        case KEY_Y:                         return "Y";
        case KEY_Z:                         return "Z";
        case KEY_PERIOD:                    return ".";
        case KEY_COMMA:                     return ",";
        case KEY_MINUS:case KEY_KP_MINUS:   return "-";
        default:                            return "";
    }
}

// Getters
KeyboardKeys Keyboard::getKeyboardInput(const unsigned int &id) const
{
    // Mapping for QWERTY
    switch (id) {
        case SDLK_0:            return KeyboardKeys::KEY_0;
        case SDLK_1:            return KeyboardKeys::KEY_1;
        case SDLK_2:            return KeyboardKeys::KEY_2;
        case SDLK_3:            return KeyboardKeys::KEY_3;
        case SDLK_4:            return KeyboardKeys::KEY_4;
        case SDLK_5:            return KeyboardKeys::KEY_5;
        case SDLK_6:            return KeyboardKeys::KEY_6;
        case SDLK_7:            return KeyboardKeys::KEY_7;
        case SDLK_8:            return KeyboardKeys::KEY_8;
        case SDLK_9:            return KeyboardKeys::KEY_9;
        case SDLK_KP_0:         return KeyboardKeys::KEY_KP_0;
        case SDLK_KP_1:         return KeyboardKeys::KEY_KP_1;
        case SDLK_KP_2:         return KeyboardKeys::KEY_KP_2;
        case SDLK_KP_3:         return KeyboardKeys::KEY_KP_3;
        case SDLK_KP_4:         return KeyboardKeys::KEY_KP_4;
        case SDLK_KP_5:         return KeyboardKeys::KEY_KP_5;
        case SDLK_KP_6:         return KeyboardKeys::KEY_KP_6;
        case SDLK_KP_7:         return KeyboardKeys::KEY_KP_7;
        case SDLK_KP_8:         return KeyboardKeys::KEY_KP_8;
        case SDLK_KP_9:         return KeyboardKeys::KEY_KP_9;
        case SDLK_a:            return KeyboardKeys::KEY_A;
        case SDLK_b:            return KeyboardKeys::KEY_B;
        case SDLK_BACKSLASH:    return KeyboardKeys::KEY_BACKSLASH;
        case SDLK_BACKSPACE:    return KeyboardKeys::KEY_BACKSPACE;
        case SDLK_c:            return KeyboardKeys::KEY_C;
        case SDLK_CAPSLOCK:     return KeyboardKeys::KEY_CAPSLOCK;
        case SDLK_d:            return KeyboardKeys::KEY_D;
        case SDLK_DELETE:       return KeyboardKeys::KEY_DELETE;
        case SDLK_DOWN:         return KeyboardKeys::KEY_DOWN;
        case SDLK_e:            return KeyboardKeys::KEY_E;
        case SDLK_ESCAPE:       return KeyboardKeys::KEY_ESCAPE;
        case SDLK_f:            return KeyboardKeys::KEY_F;
        case SDLK_F1:           return KeyboardKeys::KEY_F1;
        case SDLK_F2:           return KeyboardKeys::KEY_F2;
        case SDLK_F3:           return KeyboardKeys::KEY_F3;
        case SDLK_F4:           return KeyboardKeys::KEY_F4;
        case SDLK_F5:           return KeyboardKeys::KEY_F5;
        case SDLK_F6:           return KeyboardKeys::KEY_F6;
        case SDLK_F7:           return KeyboardKeys::KEY_F7;
        case SDLK_F8:           return KeyboardKeys::KEY_F8;
        case SDLK_F9:           return KeyboardKeys::KEY_F9;
        case SDLK_F10:          return KeyboardKeys::KEY_F10;
        case SDLK_F11:          return KeyboardKeys::KEY_F11;
        case SDLK_F12:          return KeyboardKeys::KEY_F12;
        case SDLK_g:            return KeyboardKeys::KEY_G;
        case SDLK_h:            return KeyboardKeys::KEY_H;
        case SDLK_i:            return KeyboardKeys::KEY_I;
        case SDLK_j:            return KeyboardKeys::KEY_J;
        case SDLK_k:            return KeyboardKeys::KEY_K;
        case SDLK_l:            return KeyboardKeys::KEY_L;
        case SDLK_LALT:         return KeyboardKeys::KEY_LALT;
        case SDLK_LCTRL:        return KeyboardKeys::KEY_LCTRL;
        case SDLK_LEFT:         return KeyboardKeys::KEY_LEFT;
        case SDLK_LSHIFT:       return KeyboardKeys::KEY_LSHIFT;
        case SDLK_m:            return KeyboardKeys::KEY_M;
        case SDLK_n:            return KeyboardKeys::KEY_N;
        case SDLK_NUMLOCKCLEAR: return KeyboardKeys::KEY_NUMLOCKCLEAR;
        case SDLK_o:            return KeyboardKeys::KEY_O;
        case SDLK_p:            return KeyboardKeys::KEY_P;
        case SDLK_q:            return KeyboardKeys::KEY_Q;
        case SDLK_r:            return KeyboardKeys::KEY_R;
        case SDLK_RIGHT:        return KeyboardKeys::KEY_RIGHT;
        case SDLK_RALT:         return KeyboardKeys::KEY_RALT;
        case SDLK_RCTRL:        return KeyboardKeys::KEY_RCTRL;
        case SDLK_RETURN:       return KeyboardKeys::KEY_RETURN;
        case SDLK_RSHIFT:       return KeyboardKeys::KEY_RSHIFT;
        case SDLK_s:            return KeyboardKeys::KEY_S;
        case SDLK_SPACE:        return KeyboardKeys::KEY_SPACE;
        case SDLK_t:            return KeyboardKeys::KEY_T;
        case SDLK_TAB:          return KeyboardKeys::KEY_TAB;
        case SDLK_u:            return KeyboardKeys::KEY_U;
        case SDLK_UP:           return KeyboardKeys::KEY_UP;
        case SDLK_v:            return KeyboardKeys::KEY_V;
        case SDLK_w:            return KeyboardKeys::KEY_W;
        case SDLK_x:            return KeyboardKeys::KEY_X;
        case SDLK_y:            return KeyboardKeys::KEY_Y;
        case SDLK_z:            return KeyboardKeys::KEY_Z;
        case SDLK_PERIOD:       return KeyboardKeys::KEY_PERIOD;
        case SDLK_COMMA:        return KeyboardKeys::KEY_COMMA;
        case SDLK_MINUS:        return KeyboardKeys::KEY_MINUS;
        case SDLK_KP_MINUS:     return KeyboardKeys::KEY_KP_MINUS;
        default:                return KeyboardKeys::KEY_UNKNOW;
    }
}
