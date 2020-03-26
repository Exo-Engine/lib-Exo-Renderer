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

#include <deque>
#include <cwchar>

#include "Shader.h"
#include "Label.h"
#include "Font.h"
#include "Buffer.h"

namespace	ExoRendererSDLOpenGL
{

class TextRenderer
{
public:
	TextRenderer(void);
	~TextRenderer(void);
	
	void push(ExoRenderer::Label *element);
	void render(const glm::mat4& orthographic);
private:
	void prepare(const glm::mat4& orthographic);
	static void renderCharacter(const wchar_t c, float& x, float& y, ExoRenderer::Label* label);
	static std::wstring utf8ToUtf16(const std::string& utf8Str);
public:
	static Shader* pTextShader;
	static Buffer* vaoBuffer;
	static Buffer* vertexBuffer;
private:
	std::deque<ExoRenderer::Label*> _renderQueue;
	int _currentTextureBind;
};

}

