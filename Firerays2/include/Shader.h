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

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
#endif
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "IShader.h"

namespace	ExoRendererFirerays2
{

class Shader: public ExoRenderer::IShader
{
public:
	Shader(void);
	Shader(const std::string& filePath);
	Shader(const std::vector<std::string>& shaderSource);
	~Shader(void);

	void initialize(const std::string& filePath);
	void initialize(const std::vector<std::string>& shaderSource);

	virtual void bind(void) const;
	virtual void unbind(void) const;

	// Setters
	virtual void setMat4(const std::string& name, const glm::mat4& value) const;
	virtual void setVec4(const std::string& name, const glm::vec4& value) const;
	virtual void setVec4(const std::string& name, float x, float y, float z, float w) const;
	virtual void setVec3(const std::string& name, const glm::vec3& value) const;
	virtual void setVec3(const std::string& name, float x, float y, float z) const;
	virtual void setVec2(const std::string& name, const glm::vec2& value) const;
	virtual void setVec2(const std::string& name, float x, float y) const;
	virtual void setFloat(const std::string& name, const float& value) const;
	virtual void setInt(const std::string& name, const int& value) const;

	// Getters
	GLuint getShader(void) const;
private:
	void loadShader(const std::string& filePath, std::string& vertexShaderCode, std::string& fragmentShaderCode);
	void loadShader(const std::vector<std::string>& shaderSource, std::string& vertexShaderCode, std::string& fragmentShaderCode);
	unsigned int compileShader(const std::string& shaderCode, const GLenum& type);
private:
	GLuint _programId;
};

}
