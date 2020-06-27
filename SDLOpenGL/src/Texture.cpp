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

#include "Texture.h"

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Texture::Texture(unsigned int width, unsigned int height, TextureFormat format, TextureFilter filter) :
	_width(width), _height(height)
{
	GLenum	textureFormat;

	_format = format;
	GL_CALL(glGenTextures(1, &_id));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, _id));

	applyFilter(filter);

	switch (format)
	{
		case RGBA:
			textureFormat = GL_RGBA;
			break;
		case RGB:
			textureFormat = GL_RGB;
			break;
		case DEPTH:
			textureFormat = GL_DEPTH_COMPONENT;
			break;
	}

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, NULL));
}

Texture::Texture(const std::string& filePath, TextureFilter filter)
{
	SDL_Surface*	image = IMG_Load(filePath.c_str());
	GLenum			textureFormat;

	if (!image)
	{
		image = generateDefaultTexture();
		SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 255, 0, 255));
	}

	GL_CALL(glGenTextures(1, &_id));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, _id));

	applyFilter(filter);

	textureFormat = getFormat(image->format->BytesPerPixel);

	switch (textureFormat)
	{
		case GL_RGB:
			_format = RGB;
			break;
		case GL_RGBA:
			_format = RGBA;
			break;
		case GL_RED:
			_format = DEPTH;
			break;
	}

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, image->w, image->h, 0, textureFormat, GL_UNSIGNED_BYTE, image->pixels));

	_width = image->w;
	_height = image->h;

	// Free texture
	if (image)
		SDL_FreeSurface(image);
}

Texture::~Texture(void)
{
	glDeleteTextures(1, &_id);
}

void Texture::bind(int unit) const
{
	GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, _id));
}

void Texture::unbind(void) const
{

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	GL_CALL(glActiveTexture(GL_TEXTURE0));
}

// Getters
int Texture::getEngineId(void) const
{
	return _id;
}

GLuint Texture::getBuffer(void) const
{
	return _id;
}

TextureFormat	Texture::getFormat(void) const
{
	return (_format);
}

// Private
void Texture::applyFilter(const TextureFilter& filter)
{
	switch (filter)
	{
		case TextureFilter::NEAREST:
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			break;
		default:
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			break;
	}
}

// Private
SDL_Surface* Texture::generateDefaultTexture()
{
	Uint32 rmask, gmask, bmask, amask;
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	}
	else
	{
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}

	return SDL_CreateRGBSurface(0, 1, 1, 32, rmask, gmask, bmask, amask);
}

GLenum Texture::getFormat(const unsigned int& format)
{
	switch (format)
	{
		case 4:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return GL_BGRA;
			else
				return GL_RGBA;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return GL_BGR;
			else
				return GL_RGB;
		case 1:
			return GL_DEPTH_COMPONENT;
		default:
			return GL_RGBA; // By default
	}
}

int	 Texture::getWidth(void) const
{
	return (_width);
}

int	 Texture::getHeight(void) const
{
	return (_height);
}
