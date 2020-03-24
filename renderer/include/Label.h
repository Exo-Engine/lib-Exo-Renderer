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
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "ILabel.h"
#include "Font.h"
#include "anchorPoint.h"
#include "textAlignment.h"

namespace   ExoRenderer
{

class Label : public ILabel
{
public:
    Label(void);
    ~Label(void);
    
    virtual void contextInfo(float scaleFactor, float contextWidth, float contextHeight, bool force = false);
    virtual void updateGlyph();
    
    // Getters
    virtual std::string getText(void) const;
    virtual glm::vec3 getColor(void) const;
    virtual glm::vec2 getGlyphLayout(void) const;
    virtual const std::shared_ptr<Font>& getFont(void);
    virtual float getFontScale(void) const;
    virtual TextAlignment getTextAlignment(void) const;
    
    virtual glm::vec2   getUnscaleRealPosition(void);
    virtual glm::vec2   getPosition(void) const;
    virtual glm::vec2   getVirtualOffset(void) const;
    virtual glm::vec2   getRelativeParentPosition(void) const;
    virtual glm::vec2   getMargin(void) const;
    
    virtual glm::vec2   getRealPosition(void) const;
    virtual glm::vec2   getScalePosition(void) const;
    virtual glm::vec2   getScaleVirtualOffset(void) const;
    virtual glm::vec2   getScaleRelativeParentPosition(void) const;
    
    virtual float getScaleFactor(void) const;
    virtual float getContextWidth(void) const;
    virtual float getContextHeight(void) const;
    
    // Setters
    virtual void setScale(bool scale);
    virtual void setText(const std::string& text);
    virtual void setFont(const std::shared_ptr<Font>& font);
    virtual void setFontScale(float scale);
    
    virtual void setAnchor(const AnchorPoint& anchor);
    virtual void setLocalAnchor(const AnchorPoint& anchor);
    virtual void setTextAlignment(const TextAlignment& alignment);
    virtual void setMaxWidth(float maxWidth);
    
    virtual void setPosition(float x, float y);
    virtual void setPosition(const glm::vec2& val);
    
    virtual void setVirtualOffset(float x, float y);
    virtual void setVirtualOffset(const glm::vec2& val);
    
    virtual void setRelativeParentPosition(float x, float y);
    virtual void setRelativeParentPosition(const glm::vec2& val);
    
    virtual void setMargin(float x, float y);
    virtual void setMargin(const glm::vec2& val);
    
    virtual void setColor(const glm::vec3& color);
    virtual void setColor(float r, float g, float b);
private:
    glm::vec2 calcGlyphLayout(std::string& text, const float& scale, const std::shared_ptr<Font>& font);
    glm::vec2 calcRealPosition(const glm::vec2& position, const glm::vec2& margin, const glm::vec2& size, const AnchorPoint& anchor, const AnchorPoint& localAnchor);
private:
    std::string _text;
    glm::vec3 _color;
    glm::vec2 _glyphLayout;
    std::shared_ptr<Font> _font;
    float _fontScale;
    float _maxWidth;

    // Default
    AnchorPoint _anchor;
    AnchorPoint _localAnchor;
    TextAlignment _textAlignment;

    glm::vec2 _position;
    glm::vec2 _margin;
    glm::vec2 _virtualOffset;
    glm::vec2 _relativeParentPosition;

    // Scale
    float _scaleFactor;
    float _contextWidth, _contextHeight;

    // Scale Resolution
    bool _scale;
    glm::vec2 _realPosition;

    glm::vec2 _scalePosition;
    glm::vec2 _scaleVirtualOffset;
    glm::vec2 _scaleRelativeParentPosition;
};

}
