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

#include "Material.h"

#include <SDL2/SDL_opengl.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

namespace	ExoRenderer
{
	class	Model;

	class	Bone
	{
		public:
			Bone(void);
			virtual ~Bone(void);

			const glm::mat4&	getMatrix(void) const;

		private:
			Bone(const Bone& src);

			Bone&	operator=(const Bone& src);

			glm::mat4x4				_matrix;
			glm::vec3				_direction;
	};

	class	BodyPart
	{
		public:
			typedef struct
			{
				glm::highp_uvec3	vertices;
				glm::highp_uvec3	textureVertices;
				glm::highp_uvec3	verticesnormals;
			}	Face;

			BodyPart(Model* model, BodyPart* parent);
			virtual ~BodyPart(void);

			void	addFace(const Face& face);
			void	addChild(BodyPart* bodyPart);
			void	setMaterial(Material* material);

			Model*									getModel(void) const;
			Bone&									getBone(void);
			const Bone&								getBone(void) const;
			BodyPart*								getParent(void) const;
			Material*								getMaterial(void) const;
			const std::vector<BodyPart*>&			getChilds(void) const;
			const std::vector<glm::highp_uvec3>&	getVertices(void) const;
			const std::vector<glm::highp_uvec3>&	getVerticesNormals(void) const;
			const std::vector<glm::highp_uvec3>&	getTextureVertices(void) const;
		private:
			BodyPart(void);
			BodyPart(const BodyPart& src);

			BodyPart&	operator=(const BodyPart& src);

			Model*							_model;
			BodyPart*						_parent;
			std::vector<BodyPart*>			_childs;
			Bone							_bone;
			Material*						_material;
			std::vector<glm::highp_uvec3>	_vertices;
			std::vector<glm::highp_uvec3>	_textureVertices;
			std::vector<glm::highp_uvec3>	_verticesnormals;
	};
}
