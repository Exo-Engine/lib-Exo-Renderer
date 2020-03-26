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

#include "Label.h"

using namespace ExoRenderer;

Label::Label(void)
: ILabel(), _text(""), _color(glm::vec3(1, 1, 1)), _glyphLayout(0.0f), _font(nullptr), _fontScale(1.0f), _maxWidth(0), _anchor(AnchorPoint::TOP_LEFT),
_localAnchor(AnchorPoint::CENTER), _textAlignment(TextAlignment::CENTER), _position(glm::vec2(0.0f, 0.0f)), _margin(0.0f, 0.0f),
_virtualOffset(glm::vec2(0.0f, 0.0f)), _relativeParentPosition(glm::vec2(0.0f, 0.0f)), _scaleFactor(1.0f), _contextWidth(1), _contextHeight(1),
_scale(true), _realPosition(0.0f), _scalePosition(glm::vec2(0.0f, 0.0f)), _scaleVirtualOffset(glm::vec2(0.0f, 0.0f)), _scaleRelativeParentPosition(0.0f, 0.0f)
{	}

Label::~Label(void)
{	}

void Label::contextInfo(float scaleFactor, float contextWidth, float contextHeight, bool force)
{
	if (force || _scaleFactor != scaleFactor || _contextWidth != contextWidth || _contextHeight != contextHeight)
	{
		// Update
		_scaleFactor = scaleFactor;
		_contextWidth = contextWidth;
		_contextHeight = contextHeight;
		
		// Calculate new position
		if (_scale)
		{
			_scalePosition = _position * _scaleFactor;
			_scaleVirtualOffset = _virtualOffset * _scaleFactor;
			_scaleRelativeParentPosition = _relativeParentPosition * _scaleFactor;
		}
		updateGlyph();
	}
}

void Label::updateGlyph()
{
	_glyphLayout = calcGlyphLayout(_text, getFontScale(), _font);
	if (_scale)
		_realPosition = calcRealPosition(_scalePosition, _margin * _scaleFactor, _glyphLayout, _anchor, _localAnchor);
	else
		_realPosition = calcRealPosition(_position, _margin * _scaleFactor, _glyphLayout, _anchor, _localAnchor);
}

// Getters
std::string Label::getText(void) const
{
	return _text;
}

float Label::getFontScale(void) const
{
	return _fontScale * _scaleFactor;
}

TextAlignment Label::getTextAlignment(void) const
{
	return _textAlignment;
}

glm::vec2	Label::getUnscaleRealPosition(void)
{
	return calcRealPosition(_position, _margin * _scaleFactor, _glyphLayout, _anchor, _localAnchor);
}

glm::vec2	Label::getPosition(void) const
{
	return _position;
}

glm::vec2	Label::getVirtualOffset(void) const
{
	return _virtualOffset;
}

glm::vec2	Label::getRelativeParentPosition(void) const
{
	return _relativeParentPosition;
}

glm::vec2	Label::getMargin(void) const
{
	return _margin;
}

glm::vec2 Label::getRealPosition(void) const
{
	return _realPosition;
}

glm::vec2 Label::getScalePosition(void) const
{
	return _scale ? _scalePosition : _position;
}

glm::vec2 Label::getScaleVirtualOffset(void) const
{
	return _scale ? _scaleVirtualOffset : _virtualOffset;
}

glm::vec2 Label::getScaleRelativeParentPosition(void) const
{
	return _scale ? _scaleRelativeParentPosition : _relativeParentPosition;
}

glm::vec3 Label::getColor(void) const
{
	return _color;
}

glm::vec2 Label::getGlyphLayout(void) const
{
	return _glyphLayout;
}

const std::shared_ptr<Font>& Label::getFont(void)
{
	return _font;
}

float Label::getScaleFactor(void) const
{
	return _scaleFactor;
}

float Label::getContextWidth(void) const
{
	return _contextWidth;
}

float Label::getContextHeight(void) const
{
	return _contextHeight;
}

// Setters
void Label::setScale(bool scale)
{
	_scale = scale;
}

void Label::setText(const std::string& text)
{
	_text = text;
}

void Label::setAnchor(const AnchorPoint& anchor)
{
	_anchor = anchor;
}

void Label::setLocalAnchor(const AnchorPoint& anchor)
{
	_localAnchor = anchor;
}

void Label::setTextAlignment(const TextAlignment& alignment)
{
	_textAlignment = alignment;
}

void Label::setMaxWidth(float maxWidth)
{
	_maxWidth = maxWidth;
}

void Label::setPosition(float x, float y)
{
	_position = {x, y};
}

void Label::setPosition(const glm::vec2& val)
{
	_position = val;
}

void Label::setVirtualOffset(float x, float y)
{
	_virtualOffset = {x, y};
}

void Label::setVirtualOffset(const glm::vec2& val)
{
	_virtualOffset = val;
}

void Label::setRelativeParentPosition(float x, float y)
{
	_relativeParentPosition = {x, y};
}

void Label::setRelativeParentPosition(const glm::vec2& val)
{
	_relativeParentPosition = val;
}

void Label::setMargin(float x, float y)
{
	_margin = {x, y};
}

void Label::setMargin(const glm::vec2& val)
{
	_margin = val;
}

void Label::setColor(const glm::vec3& color)
{
	setColor(color.x, color.y, color.z);
}

void Label::setColor(float r, float g, float b)
{
	_color = glm::vec3(r, g, b);
}

void Label::setFont(const std::shared_ptr<Font>& font)
{
	_font = font;
}

void Label::setFontScale(float scale)
{
	_fontScale = scale;
}

// Private
glm::vec2 Label::calcGlyphLayout(std::string& text, const float& scale, const std::shared_ptr<Font>& font)
{
	float width = 0, maxWidth = 0;
	float height = 0;
	unsigned int lastWord = 0;
	
	for (unsigned int i = 0; i < text.length(); i++)
	{
		if(text[i] == ' ') // Space
		{
			lastWord = i;
			width += 18 * scale;
		}
		else if (text[i] == '\n') // Return
		{
			maxWidth = width;
			width = 0;
			height += 85 * scale;
		}
		else
		{
			CharDescriptor ch = font->getFont()->getCharacter(text[i]);
			width += ch.xAdvance * scale;
			
			if ((ch.height + ch.yOffset) * scale > height)
				height = (ch.height + ch.yOffset) * scale;
			
			if (_maxWidth > 0 && width > _maxWidth * _scaleFactor)
			{
				text[lastWord] = '\n';
				i = lastWord - 1;
			}
		}
		
		if (width > maxWidth)
			maxWidth = width;
	}
	
	return glm::vec2(maxWidth, height);
}

glm::vec2 Label::calcRealPosition(const glm::vec2& position, const glm::vec2& margin, const glm::vec2& size, const AnchorPoint& anchor, const AnchorPoint& localAnchor)
{	
	// Anchor (screen landmark)
	static glm::vec2 anchorPosition;
	
	switch (anchor)
	{
		case AnchorPoint::TOP_CENTER:
			anchorPosition = glm::vec2(_contextWidth / 2, 0);
			break;
		case AnchorPoint::TOP_RIGHT:
			anchorPosition = glm::vec2(_contextWidth, 0);
			break;
		case AnchorPoint::BOTTOM_LEFT:
			anchorPosition = glm::vec2(0, _contextHeight);
			break;
		case AnchorPoint::BOTTOM_CENTER:
			anchorPosition = glm::vec2(_contextWidth / 2, _contextHeight);
			break;
		case AnchorPoint::BOTTOM_RIGHT:
			anchorPosition = glm::vec2(_contextWidth, _contextHeight);
			break;
		case AnchorPoint::CENTER_LEFT:
			anchorPosition = glm::vec2(0, _contextHeight / 2);
			break;
		case AnchorPoint::CENTER_RIGHT:
			anchorPosition = glm::vec2(_contextWidth, _contextHeight / 2);
			break;
		case AnchorPoint::CENTER:
			anchorPosition = glm::vec2(_contextWidth / 2, _contextHeight / 2);
			break;
		default: // Top Left
			anchorPosition = glm::vec2(0, 0);
			break;
	}
	
	// Local anchor
	switch (localAnchor)
	{
		case AnchorPoint::TOP_LEFT:
			return anchorPosition + glm::vec2(position.x + margin.x, position.y + margin.y);
		case AnchorPoint::TOP_CENTER:
			return anchorPosition + glm::vec2(position.x + margin.x, position.y + margin.y) + glm::vec2(-size.x / 2, 0);
		case AnchorPoint::TOP_RIGHT:
			return anchorPosition + position + margin + glm::vec2(-size.x, 0);
		case AnchorPoint::BOTTOM_LEFT:
			return anchorPosition + position + margin + glm::vec2(0, -size.y);
		case AnchorPoint::BOTTOM_CENTER:
			return anchorPosition + position + margin + glm::vec2(-size.x / 2, -size.y);
		case AnchorPoint::BOTTOM_RIGHT:
			return anchorPosition + position + margin + glm::vec2(-size.x, -size.y);
		case AnchorPoint::CENTER_LEFT:
			return anchorPosition + position + margin + glm::vec2(0, -size.y / 2);
		case AnchorPoint::CENTER_RIGHT:
			return anchorPosition + position + margin + glm::vec2(-size.x, -size.y / 2);
		default: // Center
			return anchorPosition + position + margin - (size / glm::vec2(2));
	};
}
