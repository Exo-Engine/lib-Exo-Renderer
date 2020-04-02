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

#include <string>
#include <vector>

#include "IWidget.h"
#include "Button.h"
#include "View.h"

#define SELECT_VIEW_MAX_HEIGHT 98

namespace	ExoRenderer
{

enum class SelectState {
	DEFAULT,
	ACTIVATE
};

class ISelect : public IWidget
{
public:
	struct Option {
		std::string value;
		std::string text;
	};
public:
	ISelect()
	: IWidget(WidgetType::SELECT)
	{ }

	virtual ~ISelect(void)
	{ }

	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType) = 0;
	virtual void updateNavigation(void) = 0;

	virtual void addOption(Option* option, bool selected = false) = 0;

	// Getters
	virtual std::string getValue(void) = 0;
	virtual Label* getLabel(void) = 0;
	virtual Button* getButton(void) = 0;
	virtual View* getView(void) = 0;
	virtual bool isOpen(void) = 0;

	// Setters
	virtual void setSelected(const std::string& select) = 0;
	virtual void setState(const SelectState &state) = 0;
	virtual void setNavigator(Navigator* navigator) = 0;
};

}
