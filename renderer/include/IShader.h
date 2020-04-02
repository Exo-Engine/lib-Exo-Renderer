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
#include <glm/glm.hpp>

namespace	ExoRenderer
{

class IShader
{
public:
	IShader(void)
	{ }

	virtual ~IShader(void)
	{	}

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	// Setters
	virtual void setMat4(const std::string &name, const glm::mat4 &value) const = 0;
	virtual void setVec3(const std::string &name, const glm::vec3 &value) const = 0;
	virtual void setVec3(const std::string &name, float x, float y, float z) const = 0;
	virtual void setVec2(const std::string &name, const glm::vec2 &value) const = 0;
	virtual void setVec2(const std::string &name, float x, float y) const = 0;
	virtual void setFloat(const std::string &name, const float &value) const = 0;
	virtual void setInt(const std::string &name, const int &value) const = 0;
};

}
