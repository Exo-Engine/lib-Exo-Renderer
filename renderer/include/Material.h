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

#include <string>

#include <glm/vec4.hpp>

namespace	ExoRenderer
{
	class	Material
	{
		public:
			Material(const std::string& name);
			virtual ~Material(void);

			const std::string&	getName(void) const;
			const glm::vec4&	getAmbiantColor(void) const;
			const glm::vec4&	getDiffuseColor(void) const;
			const glm::vec4&	getSpecularColor(void) const;
			const glm::vec4&	getEmissionColor(void) const;
			float				getSpecularExponent(void) const;
			float				getOpacity(void) const;
			const std::string&	getAmbiantTexture(void) const;
			const std::string&	getDiffuseTexture(void) const;
			const std::string&	getSpecularTexture(void) const;
			const std::string&	getEmissionTexture(void) const;

			void				setAmbiantColor(const glm::vec4& color);
			void				setDiffuseColor(const glm::vec4& color);
			void				setSpecularColor(const glm::vec4& color);
			void				setEmissionColor(const glm::vec4& color);
			void				setSpecularExponent(float exponent);
			void				setOpacity(float opacity);
			void				setAmbiantTexture(const std::string& texture);
			void				setDiffuseTexture(const std::string& texture);
			void				setSpecularTexture(const std::string& texture);
			void				setEmissionTexture(const std::string& texture);
		private:
			Material(void);
			Material(const Material& src);

			Material&	operator=(const Material& src);

			std::string	_name;
			glm::vec4	_ambiantColor;
			glm::vec4	_diffuseColor;
			glm::vec4	_specularColor;
			glm::vec4	_emissionColor;
			float		_specularExponent;
			float		_opacity;
			float		_opticalDensity;
			std::string	_ambiantTexture;
			std::string	_diffuseTexture;
			std::string	_specularTexture;
			std::string	_emissionTexture;
			std::string	_normalMap;
			std::string	_bumpMap;
			std::string	_alphaMap;
	};
}
