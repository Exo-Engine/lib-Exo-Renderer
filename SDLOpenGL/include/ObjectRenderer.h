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

#include "ModelInstance.h"
#include "BodyPartInstance.h"
#include "Camera.h"
#include "Shader.h"

#include <deque>

namespace	ExoRendererSDLOpenGL
{
	class	ObjectRenderer
	{
		public:
			ObjectRenderer(void);
			~ObjectRenderer(void);

			void add(ExoRendererSDLOpenGL::ModelInstance* object);
			void remove(ExoRendererSDLOpenGL::ModelInstance* object);
			void render(Camera* camera, const glm::mat4& perspective);

			static Shader*	pShader;

		private:
			void prepare(Camera* camera, const glm::mat4& perspective);
			static void renderObject(ExoRendererSDLOpenGL::ModelInstance* object, Shader* shader);
			static void renderBodyPart(ExoRendererSDLOpenGL::BodyPartInstance* bodyPart, Shader* shader);

			std::deque<ExoRendererSDLOpenGL::ModelInstance*>	_renderQueue;
	};
};
