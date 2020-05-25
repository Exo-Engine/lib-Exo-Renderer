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

#include "ILight.h"
#include "OGLCall.h"

namespace	ExoRendererFirerays2
{

class	Light : public ExoRenderer::ILight
{
	public:
		Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up);
		virtual ~Light(void);

		virtual void	setAmbient(float ambient);
		virtual void	setAmbient(const glm::vec3 &ambient);
		virtual const glm::vec3	&getAmbient(void) const;

		virtual void	setDiffuse(float diffuse);
		virtual void	setDiffuse(const glm::vec3 &diffuse);
		virtual const glm::vec3	&getDiffuse(void) const;

		virtual void	setPos(const glm::vec3 &pos);
		virtual void	setDir(const glm::vec3 &dir);
		virtual void	setUp(const glm::vec3 &up);

		virtual const glm::vec3	&getPos(void) const;
		virtual const glm::vec3	&getDir(void) const;
		virtual const glm::vec3	&getUp(void) const;

		const float	&getRadius(void) const;

		virtual const glm::mat4	&getView(void) const;
		virtual const glm::mat4	&getProjection(void) const = 0;
	protected:
		float		_radius;
	private:
		void		renderViewMatrix(void);

		glm::mat4	_view;
		glm::vec3	_ambient;
		glm::vec3	_diffuse;
		glm::vec3	_pos;
		glm::vec3	_dir;
		glm::vec3	_up;
};

}
