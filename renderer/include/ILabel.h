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

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Font.h"
#include "anchorPoint.h"
#include "textAlignment.h"

namespace	ExoRenderer
{

class ILabel
{
public:
	ILabel(void)
	{ }
	
	virtual ~ILabel(void)
	{ }
	
	virtual void contextInfo(float scaleFactor, float contextWidth, float contextHeight, bool force = false) = 0;
	virtual void updateGlyph() = 0;
	
	// Getters
	virtual std::string getText(void) const = 0;
	virtual glm::vec3 getColor(void) const = 0;
	virtual glm::vec2 getGlyphLayout(void) const = 0;
	virtual const std::shared_ptr<Font>& getFont(void) = 0;
	virtual float getFontScale(void) const = 0;
	virtual TextAlignment getTextAlignment(void) const = 0;
	
	virtual glm::vec2	getUnscaleRealPosition(void) = 0;
	virtual glm::vec2	getPosition(void) const = 0;
	virtual glm::vec2	getVirtualOffset(void) const = 0;
	virtual glm::vec2	getRelativeParentPosition(void) const = 0;
	virtual glm::vec2	getMargin(void) const = 0;
	
	virtual glm::vec2	getRealPosition(void) const = 0;
	virtual glm::vec2	getScalePosition(void) const = 0;
	virtual glm::vec2	getScaleVirtualOffset(void) const = 0;
	virtual glm::vec2	getScaleRelativeParentPosition(void) const = 0;
	
	virtual float getScaleFactor(void) const = 0;
	virtual float getContextWidth(void) const = 0;
	virtual float getContextHeight(void) const = 0;
	
	// Setters
	virtual void setScale(bool scale) = 0;
	virtual void setText(const std::string& text) = 0;
	virtual void setFont(const std::shared_ptr<Font>& font) = 0;
	virtual void setFontScale(float scale) = 0;
	
	virtual void setAnchor(const AnchorPoint& anchor) = 0;
	virtual void setLocalAnchor(const AnchorPoint& anchor) = 0;
	virtual void setTextAlignment(const TextAlignment& alignment) = 0;
	virtual void setMaxWidth(float maxWidth) = 0;
	
	virtual void setPosition(float x, float y) = 0;
	virtual void setPosition(const glm::vec2& val) = 0;
	
	virtual void setVirtualOffset(float x, float y) = 0;
	virtual void setVirtualOffset(const glm::vec2& val) = 0;
	
	virtual void setRelativeParentPosition(float x, float y) = 0;
	virtual void setRelativeParentPosition(const glm::vec2& val) = 0;
	
	virtual void setMargin(float x, float y) = 0;
	virtual void setMargin(const glm::vec2& val) = 0;
	
	virtual void setColor(const glm::vec3& color) = 0;
	virtual void setColor(float r, float g, float b) = 0;
};

}
