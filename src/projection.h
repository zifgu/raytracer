#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "ray.h"

class Projection {
	glm::ivec2 imageSize = glm::ivec2(0);
	glm::vec2 viewportSize = glm::vec2(0.f);
	float focalLength = 0.0f;

	// in camera coordinates
	glm::vec3 pixelUpperLeft;
	glm::vec3 pixelDelta;
public:
	Projection(glm::ivec2 imageSize, glm::vec2 viewportSize, float focalLength) : imageSize(imageSize), viewportSize(viewportSize), focalLength(focalLength) {

		glm::vec3 viewportRight = glm::vec3(viewportSize.x, 0.f, 0.f);
		glm::vec3 viewportDown = glm::vec3(0.f, -viewportSize.y, 0.f);
		pixelDelta =
			viewportRight / static_cast<float>(imageSize.x) +
			viewportDown / static_cast<float>(imageSize.y);
		glm::vec3 viewportUpperLeft =
			glm::vec3(0.f, 0.f, -focalLength)
			- 0.5f * viewportRight
			- 0.5f * viewportDown;
		pixelUpperLeft = viewportUpperLeft + 0.5f * pixelDelta;
	}

	Ray getRay(glm::ivec2 pixelCoord, bool jitter = false) const {
		// random offset in [-0.5, -0.5] to [0.5, 0.5]
		glm::vec3 offset = jitter ?
			glm::vec3(random() - 0.5f, random() - 0.5f, 0.f) :
			glm::vec3(0.f);
		glm::vec3 pixel = pixelUpperLeft + pixelDelta * (glm::vec3(pixelCoord, 0.f) + offset);
		glm::vec3 cameraOrigin = glm::vec3(0.f);
		return Ray(cameraOrigin, pixel - cameraOrigin);
	}
};