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

#include <deque>

#include "Label.h"
#include "IWidget.h"
#include "Button.h"

namespace	ExoRenderer
{

class Navigator;

enum class ViewState {
	DEFAULT,
	SELECTED
};

class IView : public IWidget
{
public:
	IView()
	: IWidget(WidgetType::VIEW)
	{ }
	
	virtual ~IView(void)
	{ }
	
	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType) = 0;
	virtual void addChild(IWidget *widget) = 0;
	virtual void addChild(ILabel *label) = 0;
	
	// Getters
	virtual const ViewState &getState(void) = 0;
	virtual const std::shared_ptr<ITexture>& getBackgroundTexture(void) = 0;
	virtual float getScrollHeight(void) = 0;
	virtual float getLastScrollHeight(void) = 0;
	virtual Button* getScrollbarButton(void) = 0;
	virtual glm::vec4& getParentScissor(void) = 0;
	
	virtual std::deque<IWidget*>& getRenderQueue(void) = 0;
	virtual std::deque<IWidget*>& getFrontRenderQueue(void) = 0;
	virtual std::deque<Label*>& getLabelRenderQueue(void) = 0;
	
	// Setters
	virtual void setState(const ViewState &state) = 0;
	virtual void setNavigator(Navigator* navigator) = 0;
	virtual void setParentScissor(int x, int y, int w, int h) = 0;
	virtual void setAutoScroll(bool autoScroll) = 0;
};

}
