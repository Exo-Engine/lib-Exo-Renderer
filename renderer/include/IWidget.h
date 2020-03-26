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

#include "Enums.h"
#include "IShader.h"
#include "anchorPoint.h"

#include "IMouse.h"
#include "IKeyboard.h"
#include "IGamepad.h"

namespace	ExoRenderer
{

typedef void (*Callback)(void *);

enum WidgetNext
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT
};

class IWidget
{
public:
	enum WidgetType
	{
		BUTTON,
		CHECKBOX,
		INPUT,
		IMAGE,
		SLIDER,
		SPINNER,
		VIEW,
		SELECT
	};

	IWidget(const WidgetType& type);
	virtual ~IWidget(void);

	virtual void update(IMouse* mouse, IKeyboard* keyboard, IGamepad* gamepad, const NavigationType &navigationType) = 0;

	template <class C>
	void setCallback(C* parent, void (C::*callback)())
	{
		memcpy(&_callback, &parent, sizeof(void *));
		memcpy(&_callbackFunction, &callback, sizeof(Callback));
	}

	// Getters
	WidgetType	getType(void) const { return _type; };
	float		getOpacity(void) { return _opacity; }
	IWidget	 *getNextWidget(WidgetNext id)
	{
		if ((int)id > 4) return nullptr;
		return _pNextWidget[(int)id];
	}
	bool		isEnabled(void) const { return _enabled; };

	glm::vec2	getUnscaleRealPosition(void) { return calcRealPosition(_position, _size, REFRENCE_RESOLUTION_WIDTH, REFRENCE_RESOLUTION_HEIGHT, _anchor, _localAnchor); }
	glm::vec2	getPosition(void) const { return _position; }
	glm::vec2	getVirtualOffset(void) const { return _virtualOffset; }
	glm::vec2	getRelativeParentPosition(void) const { return _relativeParentPosition; }
	glm::vec2	getSize(void) const { return _size; }

	glm::vec2	getRealPosition(void) const { return _realPosition; }
	glm::vec2	getScalePosition(void) const { return _scale ? _scalePosition : _position; }
	glm::vec2	getScaleSize(void) const { return _scale ? _scaleSize : _size; }

	// Setters
	void setNextWidget(WidgetNext id, IWidget *widget)
	{
		if ((int)id > 4) return;
		_pNextWidget[(int)id] = widget;
	}
	virtual void setIsEnabled(bool enabled) { _enabled = enabled; }
	virtual void setOpacity(float opacity) { _opacity = opacity; }
	virtual void setScale(bool scale) { _scale = scale; }

	virtual void setAnchor(const AnchorPoint& anchor) { _anchor = anchor; }
	virtual void setLocalAnchor(const AnchorPoint& anchor) { _localAnchor = anchor; }

	virtual void setPosition(float x, float y)
	{
		_position = {x, y};
		_scalePosition = _position * _scaleFactor;
		updatePosition();
	}
	virtual void setPosition(const glm::vec2& val)
	{
		_position = val;
		_scalePosition = _position * _scaleFactor;
		updatePosition();
	}

	virtual void setVirtualOffset(float x, float y) { _virtualOffset = {x, y}; }
	virtual void setVirtualOffset(const glm::vec2& val) { _virtualOffset = val; }

	virtual void setRelativeParentPosition(float x, float y) { _relativeParentPosition = {x, y}; }
	virtual void setRelativeParentPosition(const glm::vec2& val) { _relativeParentPosition = val; }

	virtual void setSize(float x, float y)
	{
		_size = {x, y};
		_scaleSize = _size * _scaleFactor;
		updatePosition();
	}
	
	virtual void setSize(const glm::vec2& val)
	{
		_size = val;
		_scaleSize = _size * _scaleFactor;
		updatePosition();
	}
protected:
	static glm::vec2 calcRealPosition(const glm::vec2& position, const glm::vec2& size, float contextWidth, float contextHeight, const AnchorPoint& anchor, const AnchorPoint& localAnchor)
	{
		// Anchor (screen landmark)
		static glm::vec2 anchorPosition;
		
		switch (anchor)
		{
			case AnchorPoint::TOP_CENTER:
				anchorPosition = glm::vec2(contextWidth / 2, 0);
				break;
			case AnchorPoint::TOP_RIGHT:
				anchorPosition = glm::vec2(contextWidth, 0);
				break;
			case AnchorPoint::BOTTOM_LEFT:
				anchorPosition = glm::vec2(0, contextHeight);
				break;
			case AnchorPoint::BOTTOM_CENTER:
				anchorPosition = glm::vec2(contextWidth / 2, contextHeight);
				break;
			case AnchorPoint::BOTTOM_RIGHT:
				anchorPosition = glm::vec2(contextWidth, contextHeight);
				break;
			case AnchorPoint::CENTER_LEFT:
				anchorPosition = glm::vec2(0, contextHeight / 2);
				break;
			case AnchorPoint::CENTER_RIGHT:
				anchorPosition = glm::vec2(contextWidth, contextHeight / 2);
				break;
			case AnchorPoint::CENTER:
				anchorPosition = glm::vec2(contextWidth / 2, contextHeight / 2);
				break;
			default: // Top Left
				anchorPosition = glm::vec2(0, 0);
				break;
		}
		
		// Local anchor
		switch (localAnchor)
		{
			case AnchorPoint::TOP_LEFT:
				return anchorPosition + position + size;
			case AnchorPoint::TOP_CENTER:
				return anchorPosition + position + glm::vec2(0, size.y);
			case AnchorPoint::TOP_RIGHT:
				return anchorPosition + position + glm::vec2(-size.x, size.y);
			case AnchorPoint::BOTTOM_LEFT:
				return anchorPosition + position + glm::vec2(size.x, -size.y);
			case AnchorPoint::BOTTOM_CENTER:
				return anchorPosition + position + glm::vec2(0, -size.y);
			case AnchorPoint::BOTTOM_RIGHT:
				return anchorPosition + position + glm::vec2(-size.x, -size.y);
			case AnchorPoint::CENTER_LEFT:
				return anchorPosition + position + glm::vec2(size.x, 0);
			case AnchorPoint::CENTER_RIGHT:
				return anchorPosition + position + glm::vec2(-size.x, 0);
			default: // Center
				return anchorPosition + position;
		};
		
		return position;
	}
	
	void updatePosition(void)
	{
		if (_scale)
			_realPosition = calcRealPosition(_scalePosition, _scaleSize, _contextWidth, _contextHeight, _anchor, _localAnchor);
		else
			_realPosition = calcRealPosition(_position, _size, REFRENCE_RESOLUTION_WIDTH, REFRENCE_RESOLUTION_HEIGHT, _anchor, _localAnchor);
	}
protected:
	IWidget *_pNextWidget[4];
	WidgetType _type;
	bool _enabled;

	// Default
	AnchorPoint _anchor;
	AnchorPoint _localAnchor;

	glm::vec2 _position;
	glm::vec2 _virtualOffset;
	glm::vec2 _relativeParentPosition;
	glm::vec2 _size;
	float	 _opacity;

	// Scale
	float _scaleFactor;
	float _contextWidth, _contextHeight;

	// Scale Resolution
	bool _scale;
	glm::vec2 _realPosition;

	glm::vec2 _scalePosition;
	glm::vec2 _scaleSize;

	// Callback
	void* _callback;
	Callback _callbackFunction;
};

}
