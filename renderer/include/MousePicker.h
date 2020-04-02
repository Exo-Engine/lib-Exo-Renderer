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

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "IMouse.h"

namespace	ExoRenderer
{

class MousePicker
{
	public:
		MousePicker(void)
		: _ray(0, 0, 0)
		{
		}
		virtual ~MousePicker(void)
		{
		}

		void update(IMouse *mouse, int displayW, int displayH, const glm::mat4 &lookAt, const glm::mat4 &perspective)
		{
			glm::vec2 normalizedCoords = getNormalizedCoords(mouse->x, mouse->y, displayW, displayH);
			glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
			glm::vec4 eyeCoords = toEyeCoords(clipCoords, perspective);
			_ray = toWorldCoords(lookAt, eyeCoords);
		}

		// Getters
		glm::vec3 getPointOnRay(ICamera* camera, float distance)
		{
			glm::vec3	start =	camera->getPosition();
			glm::vec3	scaledRay = glm::vec3(_ray.x * distance, _ray.y * distance, _ray.z * distance);

			return start + scaledRay;
		}

		const glm::vec3 &getRay(void) const
		{
			return _ray;
		}
	private:
		static glm::vec2 getNormalizedCoords(float mouseX, float mouseY, int displayW, int displayH)
		{
			glm::vec2 result;
			result.x = (2.0f * mouseX) / displayW - 1.0f;
			result.y = (2.0f * mouseY) / displayH - 1.0f;
			return result;
		}

		static glm::vec4 toEyeCoords(const glm::vec4 &clipCoords, const glm::mat4 &projectionMatrix)
		{
			glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
			glm::vec4 eyeCoords = invertedProjection * clipCoords;
			return glm::vec4(eyeCoords.x, -eyeCoords.y, -1.0f, 0.0f);
		}

		static glm::vec3 toWorldCoords(const glm::mat4 &viewMatrix, const glm::vec4 &eyeCoords)
		{
			glm::mat4 invertedView = glm::inverse(viewMatrix);
			glm::vec4 worldRay = invertedView * eyeCoords;
			glm::vec3 mouseRay = worldRay;
			glm::normalize(mouseRay);
			return worldRay;
		}
	private:
		glm::vec3 _ray;
};

}
