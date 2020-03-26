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

#include "IButton.h"
#include "Label.h"

namespace	ExoRenderer
{

typedef void (*Callback)(void *);

class Button : public IButton
{
public:
	Button(const std::shared_ptr<ITexture>& texture, ButtonType buttonType, bool withLabel, float scaleFactor, float contextWidth, float contextHeight);
	~Button(void);

	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType);
	virtual void updateLabel(void);
	
	void resetDefaultState(void);
	
	// Getters
	virtual bool getClickEvent(void);
	virtual bool getSliced(void);
	virtual ButtonState getState(void);
	virtual const std::shared_ptr<ITexture>& getTexture(void);
	virtual Label* getLabel(void);
	virtual unsigned int getNumberOfRows(void);
	virtual unsigned int getNumberOfColumns(void);
	virtual glm::vec2 getOffset(void);
	virtual unsigned int getTextureIndex(void);
	
	// Setters
	virtual void setSliced(bool sliced);
	virtual void setTexture(const std::shared_ptr<ITexture>& texture);
	virtual void setTextureCut(unsigned int numberOfRows, unsigned int numberOfColumns);
	virtual void setState(const ButtonState& state);
	virtual void setType(const ButtonType& buttonType);
	
	virtual void setPadding(const glm::vec2& padding);
	virtual void setPadding(float x, float y);
	
	virtual void setColor(const glm::vec3& color);
	virtual void setColor(float r, float g, float b);
	
	virtual void setSecondColor(const glm::vec3& color);
	virtual void setSecondColor(float r, float g, float b);
	
	virtual void setFont(const std::shared_ptr<Font>& font);
	virtual void setText(const std::string& text);
	virtual void setTextScale(float size);
	virtual void setTextAlignment(const TextAlignment& align);
private:
	void setTextureIndex(const unsigned int& textureIndex);
private:
	bool				_clickEvent;
	Label				*_pLabel;
	ButtonType			_buttonType;
	bool				_sliced;
	ButtonState		 _state;
	std::shared_ptr<ITexture>	_texture;
	
	unsigned int		_textureIndex;
	unsigned int		_numberOfRows, _numberOfColumns;
	glm::vec2			_offset;
	
	glm::vec3			_defaultTextColor, _secondTextColor;
};

}
