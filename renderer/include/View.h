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
#include "IView.h"
#include "Button.h"

namespace	ExoRenderer
{

class View : public IView
{
public:
	View(const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows, unsigned int numberOfColumns, float scaleFactor, float contextWidth, float contextHeight);
	View(const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows, unsigned int numberOfColumns, float scaleFactor, float contextWidth, float contextHeight);
	~View(void);
	
	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType);
	virtual void addChild(IWidget *widget);
	virtual void addChild(ILabel *label);
	
	// Getters
	virtual const ViewState &getState(void);
	virtual const std::shared_ptr<ITexture>& getBackgroundTexture(void);
	virtual float getScrollHeight(void);
	virtual float getLastScrollHeight(void);
	virtual Button* getScrollbarButton(void);
	virtual glm::vec4& getParentScissor(void);
	
	virtual std::deque<IWidget*>& getRenderQueue(void);
	virtual std::deque<IWidget*>& getFrontRenderQueue(void);
	virtual std::deque<Label*>& getLabelRenderQueue(void);
	
	// Setters
	virtual void setState(const ViewState &state);
	virtual void setNavigator(Navigator* navigator);
	virtual void setParentScissor(int x, int y, int w, int h);
	virtual void setAutoScroll(bool autoScroll);
private:
	void initialize(const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows, unsigned int numberOfColumns);
	void pushWidget(IWidget* widget);
	void calculateScrollY(float yPos);
private:
	Navigator				*_pNavigator;
	ViewState				_state;
	
	char					_mouseOut;
	bool					_autoScroll;
	char					_firstShow;
	float					_scrollY;
	float					_scrollHeight, _lastScrollHeight; // The position on Y of the last widget
	glm::vec4				_parentScissor;
	std::shared_ptr<ITexture>		_backgroundTexture;
	
	Button*				 _scrollbarButton;
	std::deque<IWidget*>	_renderQueue;
	std::deque<IWidget*>	_frontRenderQueue;
	std::deque<Label*>		_labelRenderQueue;
};

}
