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
#include "IModelInstance.h"
#include "Buffer.h"
#include "Texture.h"
#include "BodyPart.h"
#include "opencl_compatibility.h"

#include "radeon_rays.h"

namespace	ExoRendererFirerays2
{
	class	BodyPartInstance : public ExoRenderer::IBodyPartInstance
	{
		public:
			BodyPartInstance(ExoRenderer::BodyPart* bodyPart, ExoRenderer::IModelInstance* model, BodyPartInstance* parent);
			virtual ~BodyPartInstance(void);

			void				setShape(RadeonRays::Shape* shape);
			RadeonRays::Shape*	getShape(void) const;

			virtual void		translate(const glm::vec3& translation);
			virtual void		rotate(float angle, const glm::vec3& axis);

		private:
			BodyPartInstance(void);

			RadeonRays::Shape*	_shape;
	};
}
