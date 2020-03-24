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

#include "ArrayTexture.h"
#include "Texture.h"
#include <stdexcept>

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

ArrayTexture::ArrayTexture()
{   }

ArrayTexture::ArrayTexture(int width, int height, std::vector<std::string>& textures, TextureFilter filter)
{
    initialize(width, height, textures, filter);
}

ArrayTexture::~ArrayTexture(void)
{
    glDeleteTextures(1, &_id);
}

void ArrayTexture::initialize(int width, int height, std::vector<std::string>& textures, TextureFilter filter)
{
    if (textures.size() <= 0)
        throw (std::invalid_argument("cannot create ArrayTexture, number of images insufficient."));

    // Generate array texture (based on the fist image in the vector)
    SDL_Surface* image = IMG_Load(textures[0].c_str());
    if (!image)
    {
        image = Texture::generateDefaultTexture();
        SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 255, 0, 255));
    }

    GL_CALL(glGenTextures(1, &_id));

    GLenum textureFormat = Texture::getFormat(image->format->BytesPerPixel);
    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, _id));

    GL_CALL(glTexImage3D(GL_TEXTURE_2D_ARRAY,
                0,
                textureFormat,
                width, height, (GLsizei)textures.size(),
                0,
                textureFormat,
                GL_UNSIGNED_BYTE,
                nullptr));

    if (image)
        SDL_FreeSurface(image);

    // Apply sub textures
    for (int i = 0; i < (int)textures.size(); i++)
    {
        image = IMG_Load(textures[i].c_str());
        if (!image)
        {
            image = Texture::generateDefaultTexture();
            SDL_FillRect(image, NULL, SDL_MapRGB(image->format, 255, 0, 255));
        }

        GL_CALL(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                0, 0, i,
                width, height, 1,
                Texture::getFormat(image->format->BytesPerPixel),
                GL_UNSIGNED_BYTE,
                image->pixels));

        if (image)
            SDL_FreeSurface(image);
    }

    // Filter
    applyFilter(filter);
}

void ArrayTexture::bind(int unit) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, _id));
}

void ArrayTexture::unbind(void) const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
}

// Getters
unsigned int ArrayTexture::getBuffer(void) const
{
    return _id;
}

// Private
void ArrayTexture::applyFilter(const TextureFilter& filter)
{
    switch (filter)
    {
        case TextureFilter::NEAREST:
            GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            break;
        default:
            GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            break;
    }
}

const GLuint    &ArrayTexture::getId(void) const
{
    return (_id);
}
