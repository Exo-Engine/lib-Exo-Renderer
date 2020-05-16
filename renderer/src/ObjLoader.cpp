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

#include "ObjLoader.h"

#include "Model.h"

#include <fstream>
#include <iostream>

#include <string.h>

using namespace	ExoRenderer;

static size_t		countWhiteSpaces(const std::string& string, size_t start)
{
	size_t	index;

	for (index = start; index < string.length(); index++)
	{
		if (!isspace(string[index]))
			break ;
	}
	return (index - start);
}

static bool			lineStartsWith(const std::string& string, const std::string& start)
{
	if (!strncmp(string.c_str(), start.c_str(), start.length()))
		return (true);
	else
		return (false);
}

static std::string	getPath(const std::string& file)
{
	size_t	pos = file.find_last_of('/', file.length());
	if (pos == std::string::npos)
		return (file);
	return (std::string(file, 0, pos));
}

static std::vector<std::string>	explode(const std::string& string, const std::vector<std::string>& delimiters, bool allowDelimitersSerie)
{
	std::vector<std::string>	result;
	bool						found;
	size_t						minPos;
	size_t						ret;
	size_t						delimiterSize;
	size_t						i;

	found = true;
	i = 0;
	while (found)
	{
		found = false;
		minPos = std::string::npos;
		for (const auto& delimiter : delimiters)
		{
			if ((ret = string.find(delimiter, i)) != std::string::npos)
			{
				if (!found || ret < minPos)
				{
					minPos = ret;
					found = true;
					delimiterSize = delimiter.length();
				}
			}
		}
		if (found)
		{
			if (minPos != i || !allowDelimitersSerie)
			{
				result.push_back(std::string(string, i, minPos - i));
			}
			i = minPos + delimiterSize;
		}
	}
	if (i < string.length())
		result.push_back(std::string(string, i, string.length() - i));
	return (result);
}

ObjLoader::ObjLoader(void)
{
}

ObjLoader::~ObjLoader(void)
{
}

Model*	ObjLoader::loadObj(const std::string& file)
{
	Model*			model;
	BodyPart*		currentBodyPart;
	BodyPart*		newBodyPart;
	Material*		defaultMaterial;
	std::ifstream	stream;
	std::string		line;
	std::string		path;

	model = new Model();
	currentBodyPart = nullptr;
	defaultMaterial = nullptr;
	path = getPath(file);
	stream.open(file, std::ifstream::in);
	if (!stream.is_open())
	{
		throw (std::ios_base::failure("cannot open file \"" + file + "\""));
	}
	while (!std::getline(stream, line).eof())
	{
		if (line.size() > 0 && line[0] != '#')
		{
			if (lineStartsWith(line, "v") && isspace(line[1]))
				loadVertex(model, line);
			else if (lineStartsWith(line, "vt") && isspace(line[2]))
				loadTextureVertex(model, line);
			else if (lineStartsWith(line, "vn") && isspace(line[2]))
				loadVertexNormal(model, line);
			else if (lineStartsWith(line, "f") && isspace(line[1]))
			{
				if (!currentBodyPart)
				{
					newBodyPart = new BodyPart(model, currentBodyPart);
					if (!defaultMaterial)
					{
						defaultMaterial = new Material("defaultMaterial");
						defaultMaterial->setAmbiantColor(glm::vec4(1, 1, 1, 1));
						defaultMaterial->setDiffuseColor(glm::vec4(1, 1, 1, 1));
						defaultMaterial->setSpecularColor(glm::vec4(1, 1, 1, 1));
						model->addMaterial(defaultMaterial);
					}
					newBodyPart->setMaterial(defaultMaterial);
					model->setBody(newBodyPart);
					currentBodyPart = newBodyPart;
				}
				loadFace(currentBodyPart, line);
			}
			else if (lineStartsWith(line, "mtllib") && isspace(line[6]))
				loadMtl(model, line, path);
			else if (lineStartsWith(line, "usemtl") && isspace(line[6]))
			{
				newBodyPart = new BodyPart(model, currentBodyPart);
				if (currentBodyPart)
				{
					currentBodyPart->addChild(newBodyPart);
				}
				else
				{
					model->setBody(newBodyPart);
				}
				currentBodyPart = newBodyPart;

				useMtl(model, line, currentBodyPart);
			}
		}
	}
	stream.close();
	return (model);
}

void	ObjLoader::loadVertex(Model* model, const std::string& line)
{
	std::vector<std::string>	elements;
	size_t						whiteSpaces;
	glm::vec3					vertex;

	whiteSpaces = countWhiteSpaces(line, 1);
	elements = explode(std::string(line, 1 + whiteSpaces, line.length() - (1 + whiteSpaces)), {" ", "\t"}, true);
	if (elements.size() != 3)
	{
		return ;
	}
	vertex.x = std::stof(elements[0]);
	vertex.y = std::stof(elements[1]);
	vertex.z = std::stof(elements[2]);
	model->addVertex(vertex);
}

void	ObjLoader::loadTextureVertex(Model* model, const std::string& line)
{
	std::vector<std::string>	elements;
	size_t						whiteSpaces;
	glm::vec2					textureVertex;

	whiteSpaces = countWhiteSpaces(line, 2);
	elements = explode(std::string(line, 2 + whiteSpaces, line.length() - (2 + whiteSpaces)), {" ", "\t"}, true);
	if (elements.size() != 2 && elements.size() != 3)
	{
		return ;
	}
	textureVertex.x = std::stof(elements[0]);
	textureVertex.y = std::stof(elements[1]);
	model->addTextureVertex(textureVertex);
}

void	ObjLoader::loadVertexNormal(Model* model, const std::string& line)
{
	std::vector<std::string>	elements;
	size_t						whiteSpaces;
	glm::vec3					vertexNormal;

	whiteSpaces = countWhiteSpaces(line, 2);
	elements = explode(std::string(line, 2 + whiteSpaces, line.length() - (2 + whiteSpaces)), {" ", "\t"}, true);
	if (elements.size() != 3)
	{
		return ;
	}
	vertexNormal.x = std::stof(elements[0]);
	vertexNormal.y = std::stof(elements[1]);
	vertexNormal.z = std::stof(elements[2]);
	model->addVertexNormal(vertexNormal);
}

void	ObjLoader::loadFace(BodyPart* bodyPart, const std::string& line)
{
	std::vector<std::string>	groupElements;
	std::vector<std::string>	elements;
	std::string					value;
	size_t						whiteSpaces;
	BodyPart::Face				faces[2];
	size_t						i, j;

	whiteSpaces = countWhiteSpaces(line, 1);
	groupElements = explode(std::string(line, 1 + whiteSpaces, line.length() - (1 + whiteSpaces)), {" ", "\t"}, true);
	if (groupElements.size() == 3)
	{
		for (i = 0; i < 3; i++)
		{
			elements = explode(groupElements[i], {"/"}, false);
			for (j = 0; j < elements.size(); j++)
			{
				if (j == 0)
				{
					if (elements[j].empty())
						faces[0].vertices[i] = 0;
					else
						faces[0].vertices[i] = std::stoul(elements[j]) - 1;
				}
				else if (j == 1)
				{
					if (elements[j].empty())
						faces[0].textureVertices[i] = 0;
					else
						faces[0].textureVertices[i] = std::stoul(elements[j]) - 1;
				}
				else if (j == 2)
				{
					if (elements[j].empty())
						faces[0].verticesnormals[i] = 0;
					else
						faces[0].verticesnormals[i] = std::stoul(elements[j]) - 1;
				}
			}
		}
		bodyPart->addFace(faces[0]);
	}
	else if (groupElements.size() == 4)
	{
		for (i = 0; i < 4; i++)
		{
			elements = explode(groupElements[i], {"/"}, false);
			if (i < 3)
			{
				for (j = 0; j < elements.size(); j++)
				{
					if (j == 0)
					{
						if (elements[j] != "")
							faces[0].vertices[i] = std::stoul(elements[j]) - 1;
						else
							faces[0].vertices[i] = 0;
					}
					else if (j == 1)
					{
						if (elements[j] != "")
							faces[0].textureVertices[i] = std::stoul(elements[j]) - 1;
						else
							faces[0].textureVertices[i] = 0;
					}
					else if (j == 2)
					{
						if (elements[j] != "")
							faces[0].verticesnormals[i] = std::stoul(elements[j]) - 1;
						else
							faces[0].verticesnormals[i] = 0;
					}
				}
			}
			else
			{
				faces[1].vertices[0] = faces[0].vertices[0];
				faces[1].vertices[1] = faces[0].vertices[2];
				faces[1].textureVertices[0] = faces[0].textureVertices[0];
				faces[1].textureVertices[1] = faces[0].textureVertices[2];
				faces[1].verticesnormals[0] = faces[0].verticesnormals[0];
				faces[1].verticesnormals[1] = faces[0].verticesnormals[2];
				for (j = 0; j < elements.size(); j++)
				{
					if (j == 0)
					{
						if (elements[j] != "")
							faces[1].vertices[2] = std::stoul(elements[j]) - 1;
						else
							faces[1].vertices[2] = 0;
					}
					else if (j == 1)
					{
						if (elements[j] != "")
							faces[1].textureVertices[2] = std::stoul(elements[j]) - 1;
						else
							faces[1].textureVertices[2] = 0;
					}
					else if (j == 2)
					{
						if (elements[j] != "")
							faces[1].verticesnormals[2] = std::stoul(elements[j]) - 1;
						else
							faces[1].verticesnormals[2] = 0;
					}
				}
			}
		}
		bodyPart->addFace(faces[0]);
		bodyPart->addFace(faces[1]);
	}
}

Material*	ObjLoader::loadMtlMaterial(Model* model, const std::string& line)
{
	Material*		material;
	size_t			whiteSpaces;
	std::string		name;

	whiteSpaces = countWhiteSpaces(line, 6);
	name = std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	material = new Material(name);
	model->addMaterial(material);
	return (material);
}

void	ObjLoader::loadMtlAmbiantColor(Material* material, const std::string& line)
{
	std::vector<std::string>	values;
	size_t						whiteSpaces;
	glm::vec4					color;

	whiteSpaces = countWhiteSpaces(line, 2);
	values = explode(std::string(line, 2 + whiteSpaces, line.length() - (2 + whiteSpaces)), {" ", "\t"}, true);
	if (values.size() != 3)
	{
		return ;
	}
	color.x = std::stof(values[0]);
	color.y = std::stof(values[1]);
	color.z = std::stof(values[2]);
	material->setAmbiantColor(color);
}

void	ObjLoader::loadMtlDiffuseColor(Material* material, const std::string& line)
{
	std::vector<std::string>	values;
	size_t						whiteSpaces;
	glm::vec4					color;

	whiteSpaces = countWhiteSpaces(line, 2);
	values = explode(std::string(line, 2 + whiteSpaces, line.length() - (2 + whiteSpaces)), {" ", "\t"}, true);
	if (values.size() != 3)
	{
		return ;
	}
	color.x = std::stof(values[0]);
	color.y = std::stof(values[1]);
	color.z = std::stof(values[2]);
	material->setDiffuseColor(color);
}

void	ObjLoader::loadMtlSpecularColor(Material* material, const std::string& line)
{
	std::vector<std::string>	values;
	size_t						whiteSpaces;
	glm::vec4					color;

	whiteSpaces = countWhiteSpaces(line, 2);
	values = explode(std::string(line, 2 + whiteSpaces, line.length() - (2 + whiteSpaces)), {" ", "\t"}, true);
	if (values.size() != 3)
	{
		return ;
	}
	color.x = std::stof(values[0]);
	color.y = std::stof(values[1]);
	color.z = std::stof(values[2]);
	color.w = 1;
	material->setSpecularColor(color);
}

void	ObjLoader::loadMtlEmissionColor(Material* material, const std::string& line)
{
	std::vector<std::string>	values;
	size_t						whiteSpaces;
	glm::vec4					color;

	whiteSpaces = countWhiteSpaces(line, 2);
	values = explode(std::string(line, 2 + whiteSpaces, line.length() - (2 + whiteSpaces)), {" ", "\t"}, true);
	if (values.size() != 3)
	{
		return ;
	}
	color.x = std::stof(values[0]);
	color.y = std::stof(values[1]);
	color.z = std::stof(values[2]);
	color.w = 1;
	material->setEmissionColor(color);
}

void	ObjLoader::loadMtlAmbiantTexture(Material* material, const std::string& line, const std::string& path)
{
	size_t			whiteSpaces;
	std::string		file;

	whiteSpaces = countWhiteSpaces(line, 6);
	file = path + "/" + std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	material->setAmbiantTexture(file);
}

void	ObjLoader::loadMtlDiffuseTexture(Material* material, const std::string& line, const std::string& path)
{
	size_t			whiteSpaces;
	std::string		file;

	whiteSpaces = countWhiteSpaces(line, 6);
	file = path + "/" + std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	material->setDiffuseTexture(file);
}

void	ObjLoader::loadMtlSpecularTexture(Material* material, const std::string& line, const std::string& path)
{
	size_t			whiteSpaces;
	std::string		file;

	whiteSpaces = countWhiteSpaces(line, 6);
	file = path + "/" + std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	material->setSpecularTexture(file);
}

void	ObjLoader::loadMtlEmissionTexture(Material* material, const std::string& line, const std::string& path)
{
	size_t			whiteSpaces;
	std::string		file;

	whiteSpaces = countWhiteSpaces(line, 6);
	file = path + "/" + std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	material->setEmissionTexture(file);
}

void	ObjLoader::loadMtlSpecularExponent(Material* material, const std::string& line)
{
	size_t			whiteSpaces;
	std::string		strExponent;
	float			exponent;

	whiteSpaces = countWhiteSpaces(line, 6);
	strExponent = std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	exponent = std::stof(strExponent);
	material->setSpecularExponent(exponent);
}

void	ObjLoader::loadMtlOpacity(Material* material, const std::string& line)
{
	size_t			whiteSpaces;
	std::string		strExponent;
	float			opacity;

	whiteSpaces = countWhiteSpaces(line, 6);
	strExponent = std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	opacity = std::stof(strExponent);
	material->setOpacity(opacity);
}

void	ObjLoader::loadMtlTransparency(Material* material, const std::string& line)
{
	size_t			whiteSpaces;
	std::string		strExponent;
	float			transparency;

	whiteSpaces = countWhiteSpaces(line, 6);
	strExponent = std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	transparency = std::stof(strExponent);
	material->setOpacity(1 - transparency);
}

void	ObjLoader::loadMtl(Model* model, const std::string& objLine, const std::string& path)
{
	Material*		currentMaterial;
	std::ifstream	stream;
	size_t			whiteSpaces;
	std::string		file;
	std::string		line;

	whiteSpaces = countWhiteSpaces(objLine, 6);
	file = path + "/" + std::string(objLine, 6 + whiteSpaces, objLine.length() - (6 + whiteSpaces));
	stream.open(file, std::ifstream::in);
	if (!stream.is_open())
	{
		return ;
	}
	while (!std::getline(stream, line).eof())
	{
		if (line.size() > 0 && line[0] != '#')
		{
			if (lineStartsWith(line, "newmtl") && isspace(line[6]))
				currentMaterial = loadMtlMaterial(model, line);
			else if ((lineStartsWith(line, "Ka") || lineStartsWith(line, "ka")) && isspace(line[2]))
				loadMtlAmbiantColor(currentMaterial, line);
			else if ((lineStartsWith(line, "Kd") || lineStartsWith(line, "kd")) && isspace(line[2]))
				loadMtlDiffuseColor(currentMaterial, line);
			else if ((lineStartsWith(line, "Ks") || lineStartsWith(line, "ks")) && isspace(line[2]))
				loadMtlSpecularColor(currentMaterial, line);
			else if ((lineStartsWith(line, "Ke") || lineStartsWith(line, "ke")) && isspace(line[2]))
				loadMtlEmissionColor(currentMaterial, line);
			else if ((lineStartsWith(line, "map_Ka") || lineStartsWith(line, "map_ka")) && isspace(line[6]))
				loadMtlAmbiantTexture(currentMaterial, line, path);
			else if ((lineStartsWith(line, "map_Kd") || lineStartsWith(line, "map_kd")) && isspace(line[6]))
				loadMtlDiffuseTexture(currentMaterial, line, path);
			else if ((lineStartsWith(line, "map_Ks") || lineStartsWith(line, "map_ks")) && isspace(line[6]))
				loadMtlSpecularTexture(currentMaterial, line, path);
			else if ((lineStartsWith(line, "map_Ke") || lineStartsWith(line, "map_ke")) && isspace(line[6]))
				loadMtlEmissionTexture(currentMaterial, line, path);
			else if ((lineStartsWith(line, "Ns") || (lineStartsWith(line, "ns")) && isspace(line[2])))
				loadMtlSpecularExponent(currentMaterial, line);
			else if ((lineStartsWith(line, "D") || (lineStartsWith(line, "d")) && isspace(line[1])))
				loadMtlOpacity(currentMaterial, line);
			else if ((lineStartsWith(line, "Tr") || (lineStartsWith(line, "tr")) && isspace(line[2])))
				loadMtlTransparency(currentMaterial, line);
		}
	}
	stream.close();
}

void	ObjLoader::useMtl(Model* model, const std::string& line, BodyPart* bodyPart)
{
	size_t			whiteSpaces;
	std::string		name;

	whiteSpaces = countWhiteSpaces(line, 6);
	name = std::string(line, 6 + whiteSpaces, line.length() - (6 + whiteSpaces));
	bodyPart->setMaterial(model->getMaterial(name));
}
