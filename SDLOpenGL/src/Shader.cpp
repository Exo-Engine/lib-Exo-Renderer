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

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Shader.h"
#include "OGLCall.h"

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

Shader::Shader(void)
: _programId(0)
{	}

Shader::Shader(const std::string& filePath)
: Shader()
{
	initialize(filePath);
}

Shader::Shader(const std::vector<std::string>& shaderSource)
: Shader()
{
	initialize(shaderSource);
}

Shader::~Shader(void)
{
	glDeleteProgram(_programId);
}

void Shader::initialize(const std::string& filePath)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	// Read the shader file
	loadShader(filePath, vertexShaderCode, fragmentShaderCode);

	// Compile
	unsigned int vertexShaderID = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	// Create the program
	GL_CALL(_programId = glCreateProgram());

	GL_CALL(glAttachShader(_programId, vertexShaderID));
	GL_CALL(glAttachShader(_programId, fragmentShaderID));
	GL_CALL(glLinkProgram(_programId));

	GLint isLinked = 0;
	GL_CALL(glGetProgramiv(_programId, GL_LINK_STATUS, &isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		GL_CALL(glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength));

		std::vector<char> errorMessage(maxLength);
		GL_CALL(glGetProgramInfoLog(_programId, maxLength, &maxLength, &errorMessage[0]));
		glDeleteProgram(_programId);
		throw (std::invalid_argument(&errorMessage[0]));
	}

	GL_CALL(glDetachShader(_programId, vertexShaderID));
	GL_CALL(glDetachShader(_programId, fragmentShaderID));

	GL_CALL(glDeleteShader(vertexShaderID));
	GL_CALL(glDeleteShader(fragmentShaderID));
}

void Shader::initialize(const std::vector<std::string>& shaderSource)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	// Read the shader file
	loadShader(shaderSource, vertexShaderCode, fragmentShaderCode);

	// Compile
	unsigned int vertexShaderID = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	// Create the program
	GL_CALL(_programId = glCreateProgram());

	GL_CALL(glAttachShader(_programId, vertexShaderID));
	GL_CALL(glAttachShader(_programId, fragmentShaderID));
	GL_CALL(glLinkProgram(_programId));

	GLint isLinked = 0;
	GL_CALL(glGetProgramiv(_programId, GL_LINK_STATUS, &isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		GL_CALL(glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength));

		std::vector<char> errorMessage(maxLength);
		GL_CALL(glGetProgramInfoLog(_programId, maxLength, &maxLength, &errorMessage[0]));
		glDeleteProgram(_programId);
		throw (std::invalid_argument(&errorMessage[0]));
	}

	GL_CALL(glDetachShader(_programId, vertexShaderID));
	GL_CALL(glDetachShader(_programId, fragmentShaderID));

	GL_CALL(glDeleteShader(vertexShaderID));
	GL_CALL(glDeleteShader(fragmentShaderID));
}

void Shader::bind(void) const
{
	GL_CALL(glUseProgram(_programId));
}

void Shader::unbind(void) const
{
	GL_CALL(glUseProgram(0));
}

// Setters
void Shader::setMat4(const std::string& name, const glm::mat4 &value) const
{
	GL_CALL(glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	GL_CALL(glUniform4fv(glGetUniformLocation(_programId, name.c_str()), 1, &value[0]));
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	GL_CALL(glUniform4f(glGetUniformLocation(_programId, name.c_str()), x, y, z, w));
}

void Shader::setVec3(const std::string& name, const glm::vec3 &value) const
{
	GL_CALL(glUniform3fv(glGetUniformLocation(_programId, name.c_str()), 1, &value[0]));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	GL_CALL(glUniform3f(glGetUniformLocation(_programId, name.c_str()), x, y, z));
}

void Shader::setVec2(const std::string& name, const glm::vec2 &value) const
{
	GL_CALL(glUniform2fv(glGetUniformLocation(_programId, name.c_str()), 1, &value[0]));
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
	GL_CALL(glUniform2f(glGetUniformLocation(_programId, name.c_str()), x, y));
}

void Shader::setFloat(const std::string& name, const float &value) const
{
	GL_CALL(glUniform1f(glGetUniformLocation(_programId, name.c_str()), value));
}

void Shader::setInt(const std::string& name, const int &value) const
{
	GL_CALL(glUniform1i(glGetUniformLocation(_programId, name.c_str()), value));
}

// Getters
GLuint Shader::getShader(void) const
{
	return _programId;
}

// Private
void Shader::loadShader(const std::string& filePath, std::string &vertexShaderCode, std::string &fragmentShaderCode)
{
	// Read the shader file
	std::fstream file(filePath);
	if(!file.is_open())
		throw (std::invalid_argument("Error: Shader file '" + filePath + "' not found!"));

	std::string line;
	bool inFragmentShader = false;

	while (std::getline(file, line))
	{
		if (line == "#FRAGMENT")
			inFragmentShader = true;
		else if (inFragmentShader)
		{
			fragmentShaderCode += '\n';
			fragmentShaderCode += line;
		}
		else
		{
			vertexShaderCode += '\n';
			vertexShaderCode += line;
		}
	}
}

void Shader::loadShader(const std::vector<std::string>& shaderSource, std::string& vertexShaderCode, std::string& fragmentShaderCode)
{
	bool inFragmentShader = false;

	for (const std::string& line : shaderSource)
	{
		if (line == "#FRAGMENT")
			inFragmentShader = true;
		else if (inFragmentShader)
		{
			fragmentShaderCode += '\n';
			fragmentShaderCode += line;
		}
		else
		{
			vertexShaderCode += '\n';
			vertexShaderCode += line;
		}
	}
}

unsigned int Shader::compileShader(const std::string& shaderCode, const GLenum& type)
{
	unsigned int shaderId = glCreateShader(type);
	const char *c_str = shaderCode.c_str();
	GL_CALL(glShaderSource(shaderId, 1, &c_str, NULL));
	GL_CALL(glCompileShader(shaderId));

	// Check
	GLint result = GL_FALSE;
	int InfoLogLength;


	GL_CALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));
	GL_CALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength));
	if (result != GL_TRUE) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		throw (std::invalid_argument(&VertexShaderErrorMessage[0]));
	}

	return shaderId;
}

