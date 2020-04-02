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

#include "IWidget.h"
#include "ITexture.h"
#include "IMouse.h"
#include "IKeyboard.h"
#include "IGamepad.h"
#include <memory>

namespace	ExoRenderer
{

class ICheckbox : public IWidget
{
public:
	ICheckbox()
	: IWidget(WidgetType::CHECKBOX)
	{ }

	virtual ~ICheckbox(void)
	{ }

	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType) = 0;

	// Getters
	virtual bool getChecked(void) = 0;
	virtual const std::shared_ptr<ITexture>& getTexture(void) = 0;
	virtual glm::vec2 getOffset(void) = 0;

	// Setters
	virtual void setTexture(const std::shared_ptr<ITexture>& texture) = 0;
	virtual void setChecked(bool checked) = 0;
	virtual void setSelected(bool selected) = 0;
};

}
