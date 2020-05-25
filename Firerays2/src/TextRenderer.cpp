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

#include "TextRenderer.h"
#include "RendererFirerays.h"
#include "FntLoader.h"

#include <locale>
#include <codecvt>

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

Shader* TextRenderer::pTextShader = nullptr;
Buffer* TextRenderer::vaoBuffer = nullptr;
Buffer* TextRenderer::vertexBuffer = nullptr;

TextRenderer::TextRenderer(void)
: _currentTextureBind(-1)
{	}

TextRenderer::~TextRenderer(void)
{	}

void TextRenderer::add(Label *element)
{
	_renderQueue.push_back(element);
}

void TextRenderer::remove(Label *element)
{
	_renderQueue.push_back(element);
}

void TextRenderer::render(const glm::mat4& orthographic)
{
	prepare(orthographic);

	static float x = 0;
	static float y = 0;

	for (Label* label : _renderQueue)
	{
		int engineId = label->getFont()->getTexture()->getEngineId();
		if (_currentTextureBind != engineId || _currentTextureBind == -1)
			label->getFont()->getTexture()->bind();

		pTextShader->setVec3("textColor", label->getColor());
		x = label->getRealPosition().x + label->getVirtualOffset().x + label->getRelativeParentPosition().x;
		y = label->getRealPosition().y + label->getVirtualOffset().y + label->getRelativeParentPosition().y;

		for (const auto& c : utf8ToUtf16(label->getText()))
			renderCharacter(c, x, y, label);
	}

	_currentTextureBind = -1;
}

// Private
void TextRenderer::prepare(const glm::mat4& orthographic)
{
	pTextShader->bind();
	pTextShader->setMat4("projection", orthographic);

	// Render
	vaoBuffer->bind();
}

void TextRenderer::renderCharacter(const wchar_t c, float& x, float& y, Label* label)
{
	if (c == ' ') // Space
		x += 18 * label->getFontScale();
	else if (c == '\n') // Return
	{
		x = label->getRealPosition().x + label->getVirtualOffset().x + label->getRelativeParentPosition().x;
		y += 85 * label->getFontScale();
	}
	else
	{
		CharDescriptor ch = label->getFont()->getFont()->getCharacter(c);

		float w = ch.width * label->getFontScale();
		float h = ch.height * label->getFontScale();

		float xpos = x + ch.xOffset * label->getFontScale();
		float ypos = y + (ch.height + ch.yOffset) * label->getFontScale();

		// Vertices for character
		float vertices[24] = {
			xpos,	 ypos - h,	ch.x,					ch.y,
			xpos,	 ypos,		ch.x,					ch.yMaxTextureCoord,
			xpos + w, ypos,		ch.xMaxTextureCoord,	ch.yMaxTextureCoord,

			xpos,	 ypos - h,	ch.x,					ch.y,
			xpos + w, ypos,		ch.xMaxTextureCoord,	ch.yMaxTextureCoord,
			xpos + w, ypos - h,	ch.xMaxTextureCoord,	ch.y
		};

		vertexBuffer->updateSubData(24, vertices);
		vertexBuffer->bind();

		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
		x += ch.xAdvance * label->getFontScale();
	}
}

std::wstring TextRenderer::utf8ToUtf16(const std::string& utf8Str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(utf8Str);
}
