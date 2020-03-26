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

#include <SDL2/SDL_image.h>

#include "Enums.h"
#include "ITexture.h"
#include "OGLCall.h"

namespace	ExoRendererSDLOpenGL
{

class Texture: public ExoRenderer::ITexture
{
	public:
		Texture(unsigned int width, unsigned int height, ExoRenderer::TextureFormat format, ExoRenderer::TextureFilter filter);
		Texture(const std::string& filePath, ExoRenderer::TextureFilter filter);
		virtual ~Texture(void);

		virtual void	bind(int unit = 0) const;
		virtual void	unbind(void) const;

		// Getters
		virtual int		getEngineId(void) const;
		GLuint			getBuffer(void) const;
		ExoRenderer::TextureFormat	getFormat(void) const;

		// Static
		static SDL_Surface*	generateDefaultTexture();
		static GLenum		getFormat(const unsigned int& format);

		virtual int getWidth(void) const;
		virtual int getHeight(void) const;
	private:
		static void		applyFilter(const ExoRenderer::TextureFilter& filter);
		GLuint			_id;
		ExoRenderer::TextureFormat	_format;
		int				_width;
		int				_height;
};

}
