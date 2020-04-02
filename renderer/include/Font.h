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

#include "FntLoader.h"
#include "ITexture.h"
#include "IResource.h"

#include <memory>

namespace	ExoRenderer
{

class Font : public ExoEngine::IResource
{
public:
	Font(void)
	{ }

	Font(const std::shared_ptr<FntLoader>& loader, const std::shared_ptr<ITexture>& texture)
	{
		_pLoader = loader;
		_pTexture = texture;
	}

	virtual ~Font(void)
	{ }

	// Getters
	const std::shared_ptr<FntLoader>& getFont() const { return _pLoader; }
	const std::shared_ptr<ITexture>& getTexture() const { return _pTexture; }

	// Setters
	void setFont(const std::shared_ptr<FntLoader>& loader) { _pLoader = loader; }
	void setTexture(const std::shared_ptr<ITexture>& texture) { _pTexture = texture; };

	// Operators
	inline bool operator== (const Font& font) const
	{
		if ((_pLoader.get() == font.getFont().get()) && (_pTexture.get() == font.getTexture().get())) // Compare memory address
			return true;

		return false;
	}

	inline bool operator< (const Font& font) const
	{
		if ((_pLoader.get() == font.getFont().get()) && (_pTexture.get() == font.getTexture().get())) // Compare memory address
			return false;

		return true;
	}
private:
	std::shared_ptr<FntLoader> _pLoader;
	std::shared_ptr<ITexture>	_pTexture;
};

}
