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

#include "IInput.h"
#include "Label.h"

namespace	ExoRenderer
{

class Input : public IInput
{
public:
	Input(const InputType &type, const std::shared_ptr<ITexture> &texture, const std::string &text, float scaleFactor, float contextWidth, float contextHeight);
	~Input(void);
	
	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType);
	virtual void updateLabel(void);
	
	// Getters
	virtual InputType &getType(void);
	
	virtual bool getSliced(void);
	virtual bool getSelected(void);
	virtual const std::shared_ptr<ITexture>& getTexture(void);
	virtual std::string getText(void);
	virtual Label* getLabel(void);
	virtual unsigned int getNumberOfRows(void);
	virtual unsigned int getNumberOfColumns(void);
	virtual unsigned int getTextureIndex(void);
	
	// Setters
	virtual void setType(const InputType &type);
	virtual void setMax(float max);
	virtual void setMin(float min);
	virtual void setMaxLength(int max);
	
	virtual void setSliced(bool sliced);
	virtual void setSelected(bool selected);
	virtual void setTexture(const std::shared_ptr<ITexture>& texture);
	virtual void setTextureCut(unsigned int numberOfRows, unsigned int numberOfColumns);
		
	virtual void setPadding(const glm::vec2& padding);
	virtual void setPadding(float x, float y);

	virtual void setColor(const glm::vec3& color);
	virtual void setColor(float r, float g, float b);
	
	virtual void setFont(const std::shared_ptr<Font>& font);
	virtual void setText(const std::string& text);
	virtual void setTextScale(float scale);
private:
	void insertCharacter(IKeyboard* keyboard, const char *c);
	void overflow();
private:
	Label				*_pLabel;
	InputType			_type;
	float				_min, _max;
	int				 _maxLength;
	
	bool				_sliced;
	unsigned int		_numberOfRows, _numberOfColumns;
	unsigned int		_textureIndex;
	bool				_isSelected;
	std::string		 _text;
	unsigned int		_positionInLabel;
	std::shared_ptr<ITexture>	_pTexture;
};

}
