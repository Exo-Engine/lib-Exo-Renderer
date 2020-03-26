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

#include "Light.h"

namespace	ExoRendererSDLOpenGL
{

class	OrthogonalLight : public Light
{
	public:
		OrthogonalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const glm::vec2 &x, const glm::vec2 &y, const glm::vec2 &z);
		virtual ~OrthogonalLight(void);

		void	setX(const glm::vec2 &x);
		void	setY(const glm::vec2 &y);
		void	setZ(const glm::vec2 &z);

		const glm::vec2	&getX(void) const;
		const glm::vec2	&getY(void) const;
		const glm::vec2	&getZ(void) const;

		const glm::mat4	&getProjection(void) const;
	private:
		void	renderProjection(void);

		glm::mat4	_projection;
		glm::vec2	_x;
		glm::vec2	_y;
		glm::vec2	_z;
};

}
