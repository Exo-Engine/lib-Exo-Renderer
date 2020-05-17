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

#include "IBodyPartInstance.h"

#include <glm/vec3.hpp>

namespace	ExoRenderer
{
	class	Model;
	class	IBodyPartInstance;

	class	IModelInstance
	{
		public:
			IModelInstance(Model* model);
			virtual ~IModelInstance(void);

			void				setBody(IBodyPartInstance* body);

			Model*				getModel(void) const;
			IBodyPartInstance*	getBody(void) const;

			void				translate(const glm::vec3& translation) { _body->translate(translation); }
			void				rotate(float angle, const glm::vec3& axis) { _body->rotate(angle, axis); }

		protected:
			IModelInstance(void);

		private:

			Model*				_model;
			IBodyPartInstance*	_body;
	};
}
