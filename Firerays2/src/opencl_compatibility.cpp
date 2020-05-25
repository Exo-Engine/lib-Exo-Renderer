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

#include "opencl_compatibility.h"

using namespace	ExoRendererFirerays2;

cl_float2	ExoRendererFirerays2::toCLVec2(const glm::vec2& glVector)
{
	return ((cl_float2){glVector.x, glVector.y});
}

glm::vec2	ExoRendererFirerays2::toVec2(const cl_float2& clVector)
{
	return (glm::vec2(clVector.s[0], clVector.s[1]));
}

cl_float3	ExoRendererFirerays2::toCLVec3(const glm::vec3& glVector)
{
	return ((cl_float3){glVector.x, glVector.y, glVector.z});
}

glm::vec3	ExoRendererFirerays2::toVec3(const cl_float3& clVector)
{
	return (glm::vec3(clVector.s[0], clVector.s[1], clVector.s[2]));
}

cl_float4	ExoRendererFirerays2::toCLVec4(const glm::vec4& glVector)
{
	return ((cl_float4){glVector.x, glVector.y, glVector.z, glVector.w});
}

glm::vec4	ExoRendererFirerays2::toVec4(const cl_float4& clVector)
{
	return (glm::vec4(clVector.s[0], clVector.s[1], clVector.s[2], clVector.s[3]));
}

glm::mat4x4			ExoRendererFirerays2::toMatrix(const RadeonRays::matrix& matrix)
{
	glm::mat4x4	result;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result[i][j] = matrix.m[i][j];
	return (result);
}

RadeonRays::matrix	ExoRendererFirerays2::toRRMatrix(const glm::mat4x4& matrix)
{
	RadeonRays::matrix	result;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result.m[i][j] = matrix[i][j];
	return (result);
}
