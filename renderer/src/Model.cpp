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

#include "Model.h"

using namespace	ExoRenderer;

Model::Model(void) :
	_body(nullptr)
{
}

Model::Model(const Model& src)
{
}

/*Model::~Model(void)
{
}*/

Model&		Model::operator=(const Model& src)
{
}

void		Model::setBody(BodyPart* body)
{
	_body = body;
}

BodyPart*	Model::getBody(void)
{
	return (_body);
}

void		Model::addVertex(const glm::vec3& vertice)
{
	_vertices.push_back(vertice);
}

void		Model::addVertexNormal(const glm::vec3& verticeNormal)
{
	_verticesNormals.push_back(verticeNormal);
}

void		Model::addTextureVertex(const glm::vec2& textureVertice)
{
	_textureVertices.push_back(textureVertice);
}

void		Model::addMaterial(Material* material)
{
	_materials[material->getName()] = material;
}

Material*	Model::getMaterial(const std::string& name) const
{
	try
	{
		return (_materials.at(name));
	}
	catch (const std::exception& e)
	{
		return (nullptr);
	}
}

const std::map<std::string, Material*>&	Model::getMaterials(void) const
{
	return (_materials);
}

const std::vector<glm::vec3>&	Model::getVertices(void) const
{
	return (_vertices);
}

const std::vector<glm::vec3>&	Model::getVerticesNormals(void) const
{
	return (_verticesNormals);
}

const std::vector<glm::vec2>&	Model::getTextureVertices(void) const
{
	return (_textureVertices);
}
