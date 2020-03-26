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

#include "IWidget.h"
#include "Label.h"

namespace	ExoRenderer
{

enum class InputType {
	TEXT,
	PASSWORD,
	INTEGER,
	DOUBLE
};

class IInput : public IWidget
{
public:
	IInput()
	: IWidget(WidgetType::INPUT)
	{ }
	
	virtual ~IInput(void)
	{ }
	
	template <class C>
	void addTarget(C* parent, void (C::*callback)())
	{
		memcpy(&_callback, &parent, sizeof(void *));
		memcpy(&_callbackFunction, &callback, sizeof(Callback));
	}
	
	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType) = 0;
	virtual void updateLabel(void) = 0;
	
	// Getters
	virtual InputType &getType(void) = 0;
	
	virtual bool getSliced(void) = 0;
	virtual bool getSelected(void) = 0;
	virtual const std::shared_ptr<ITexture>& getTexture(void) = 0;
	virtual std::string getText(void) = 0;
	virtual Label* getLabel(void) = 0;
	virtual unsigned int getNumberOfRows(void) = 0;
	virtual unsigned int getNumberOfColumns(void) = 0;
	virtual unsigned int getTextureIndex(void) = 0;
	
	// Setters
	virtual void setType(const InputType &type) = 0;
	virtual void setMax(float max) = 0;
	virtual void setMin(float min) = 0;
	virtual void setMaxLength(int max) = 0;
	
	virtual void setSliced(bool sliced) = 0;
	virtual void setSelected(bool selected) = 0;
	virtual void setTexture(const std::shared_ptr<ITexture>& texture) = 0;
	virtual void setTextureCut(unsigned int numberOfRows, unsigned int numberOfColumns) = 0;
		
	virtual void setPadding(const glm::vec2& padding) = 0;
	virtual void setPadding(float x, float y) = 0;

	virtual void setColor(const glm::vec3& color) = 0;
	virtual void setColor(float r, float g, float b) = 0;
	
	virtual void setFont(const std::shared_ptr<Font>& font) = 0;
	virtual void setText(const std::string& text) = 0;
	virtual void setTextScale(float scale) = 0;
};

}
