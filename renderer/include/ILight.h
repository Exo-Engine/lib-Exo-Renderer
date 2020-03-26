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

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace	ExoRenderer
{

class	ILight
{
	public:
		enum	eLightType
		{
			DEFAULT_LIGHT,
			POINT_LIGHT
		};

		ILight(const eLightType &type) : _type(type) {};
		virtual ~ILight(void) {};

		virtual void			setAmbient(float ambient) = 0;
		virtual void			setAmbient(const glm::vec3 &ambient) = 0;
		virtual const glm::vec3	&getAmbient(void) const = 0;

		virtual void			setDiffuse(float diffuse) = 0;
		virtual void			setDiffuse(const glm::vec3 &diffuse) = 0;
		virtual const glm::vec3	&getDiffuse(void) const = 0;

		virtual void			setPos(const glm::vec3 &pos) = 0;
		virtual void			setDir(const glm::vec3 &dir) = 0;
		virtual void			setUp(const glm::vec3 &up) = 0;

		virtual const glm::vec3	&getPos(void) const = 0;
		virtual const glm::vec3	&getDir(void) const = 0;
		virtual const glm::vec3	&getUp(void) const = 0;

		const eLightType		&getType(void) const;
	protected:
		eLightType	_type;
};

}
