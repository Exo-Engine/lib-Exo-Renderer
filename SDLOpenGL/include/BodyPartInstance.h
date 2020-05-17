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

namespace	ExoRendererSDLOpenGL
{
	typedef struct	s_bodyPartData
	{
		Buffer*		_vao;
		Buffer*		_vertexBuffer;
		Buffer*		_textureVertexBuffer;
		Buffer*		_normalBuffer;
		Texture*	_ambiantTexture;
		Texture*	_diffuseTexture;
		Texture*	_specularTexture;
	}				t_bodyPartData;

	class	BodyPartInstance : public ExoRenderer::IBodyPartInstance
	{
		public:
			BodyPartInstance(ExoRenderer::BodyPart* bodyPart, ExoRenderer::IModelInstance* model, BodyPartInstance* parent);
			virtual ~BodyPartInstance(void);

			//	graphics buffers and textures are shared by all instances, those functions are used by convenience

			Buffer*		getVao(void) const;
			Buffer*		getVertexBuffer(void) const;
			Buffer*		getTextureVertexBuffer(void) const;
			Buffer*		getNormalBuffer(void) const;

			Texture*	getAmbientTexture(void) const;
			Texture*	getDiffuseTexture(void) const;
			Texture*	getSpecularTexture(void) const;

		private:
			BodyPartInstance(void);
	};
}
