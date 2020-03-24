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

#pragma once

#include <glm/vec2.hpp>
#include "IArrayTexture.h"
#include <memory>

namespace   ExoRenderer
{

enum FlipSprite {
    DEFAULT,
    HORIZONTAL,
    VERTICAL
};

struct sprite
{
    // Variables
    glm::vec2 position;
    glm::vec2 scale;
    float angle;
    int layer;
    FlipSprite flip;
    
    std::shared_ptr<IArrayTexture> texture;
    std::shared_ptr<IArrayTexture> normalMapTexture;

    // Constructor
    sprite()
    : position(glm::vec2(0.0f)), scale(glm::vec2(1.0f)), angle(0.0f), layer(0), texture(nullptr), normalMapTexture(nullptr)
    {
    }
    
    sprite(std::shared_ptr<IArrayTexture> texture, std::shared_ptr<IArrayTexture> normalMapTexture, int layer = 0)
    : position(glm::vec2(0.0f)), scale(glm::vec2(1.0f)), angle(0.0f), layer(layer), flip(DEFAULT), texture(texture), normalMapTexture(normalMapTexture)
    {   }

    sprite  &operator=(const sprite &b)
    {
        position = b.position;
        scale = b.scale;
        angle = b.angle;
        layer = b.layer;
        flip = b.flip;
        texture = b.texture;
        normalMapTexture = b.normalMapTexture;
        return (*this);
    }
};

}
