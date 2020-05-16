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

#include "Material.h"

using namespace	ExoRenderer;

Material::Material(void) :
	_emissionColor(0, 0, 0, 0)
{
}

Material::Material(const std::string& name) :
	_name(name),
	_emissionColor(0, 0, 0, 0)
{
}

Material::Material(const Material& src)
{
}

Material&	Material::operator=(const Material& src)
{
	return (*this);
}

Material::~Material(void)
{
}

const std::string&	Material::getName(void) const
{
	return (_name);
}

const glm::vec4&	Material::getAmbiantColor(void) const
{
	return (_ambiantColor);
}

const glm::vec4&	Material::getDiffuseColor(void) const
{
	return (_diffuseColor);
}

const glm::vec4&	Material::getSpecularColor(void) const
{
	return (_specularColor);
}

const glm::vec4&	Material::getEmissionColor(void) const
{
	return (_emissionColor);
}

float				Material::getSpecularExponent(void) const
{
	return (_specularExponent);
}

float				Material::getOpacity(void) const
{
	return (_opacity);
}

const std::string&	Material::getAmbiantTexture(void) const
{
	return (_ambiantTexture);
}

const std::string&	Material::getDiffuseTexture(void) const
{
	return (_diffuseTexture);
}

const std::string&	Material::getSpecularTexture(void) const
{
	return (_specularTexture);
}

const std::string&	Material::getEmissionTexture(void) const
{
	return (_emissionTexture);
}

void				Material::setAmbiantColor(const glm::vec4& color)
{
	_ambiantColor = color;
}

void				Material::setDiffuseColor(const glm::vec4& color)
{
	_diffuseColor = color;
}

void				Material::setSpecularColor(const glm::vec4& color)
{
	_specularColor = color;
}

void				Material::setEmissionColor(const glm::vec4& color)
{
	_emissionColor = color;
}

void				Material::setSpecularExponent(float exponent)
{
	_specularExponent = exponent;
}

void				Material::setOpacity(float opacity)
{
	_opacity = opacity;
}

void				Material::setAmbiantTexture(const std::string& texture)
{
	_ambiantTexture = texture;
}

void				Material::setDiffuseTexture(const std::string& texture)
{
	_diffuseTexture = texture;
}

void				Material::setSpecularTexture(const std::string& texture)
{
	_specularTexture = texture;
}

void				Material::setEmissionTexture(const std::string& texture)
{
	_emissionTexture = texture;
}
