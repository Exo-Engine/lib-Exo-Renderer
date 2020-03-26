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

#include "Cursor.h"

#include "RendererSDLOpenGL.h"
#include "Image.h"

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Cursor::Cursor()
: ICursor()
{	}

Cursor::~Cursor()
{	}

void Cursor::update(void)
{
	if (_state != CursorState::HIDDEN)
	{
		if (_pImage)
			_pImage->setPosition(RendererSDLOpenGL::Get().getMouse()->x, RendererSDLOpenGL::Get().getMouse()->y);
	}
}

void Cursor::draw(GUIRenderer* renderer, const glm::mat4& orthographic)
{
	if (_pImage && _state != CursorState::HIDDEN)
	{
		renderer->push(_pImage);
		renderer->render(orthographic);
	}
}

// Setters
void Cursor::setCursorTexture(const std::shared_ptr<ITexture> &texture)
{
	if (_pImage)
		delete _pImage;
	
	_pImage = new Image(texture, RendererSDLOpenGL::Get().getUIScaleFactor(), RendererSDLOpenGL::Get().getWindow()->getWidth(), RendererSDLOpenGL::Get().getWindow()->getHeight());
	_pImage->setScale(false);
	_pImage->setLocalAnchor(AnchorPoint::TOP_LEFT);
	_pImage->setSize(12, 12);
}
