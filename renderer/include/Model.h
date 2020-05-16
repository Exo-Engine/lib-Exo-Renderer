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
#include "BodyPart.h"
#include "IResource.h"

#include <vector>
#include <memory>
#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace	ExoRenderer
{
	class	Model : public ExoEngine::IResource
	{
		public:
			Model(void);
			virtual ~Model(void) {}

			void		setBody(BodyPart* body);
			BodyPart*	getBody(void);
			void		addVertex(const glm::vec3& vertice);
			void		addVertexNormal(const glm::vec3& verticeNormal);
			void		addTextureVertex(const glm::vec2& textureVertice);

			void		addMaterial(Material* material);
			Material*	getMaterial(const std::string& name) const;

			const std::map<std::string, Material*>&	getMaterials(void) const;

			const std::vector<glm::vec3>&	getVertices(void) const;
			const std::vector<glm::vec3>&	getVerticesNormals(void) const;
			const std::vector<glm::vec2>&	getTextureVertices(void) const;
		private:
			Model(const Model& src);

			Model&	operator=(const Model& src);

			BodyPart*							_body;
			std::vector<glm::vec3>				_vertices;
			std::vector<glm::vec3>				_verticesNormals;
			std::vector<glm::vec2>				_textureVertices;
			std::map<std::string, Material*>	_materials;
	};
}
