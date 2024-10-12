#pragma once

#include "common.h"
#include "ray.h"

class Camera {
public:
	// Camera transformation
	class Frame {
		// Camera position + axes in world coordinates.
		glm::vec3 cameraPos;
		glm::vec3 cameraX;
		glm::vec3 cameraY;
		glm::vec3 cameraZ;
	public:
		Frame(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up = glm::vec3(0.f, 1.f, 0.f)) {
			cameraPos = lookFrom;
			cameraZ = glm::normalize(lookFrom - lookAt);	// camera looks down the negative z axis
			cameraX = glm::normalize(glm::cross(up, cameraZ));
			cameraY = glm::cross(cameraZ, cameraX);
		}
		const glm::vec3& x() const { return cameraX; }
		const glm::vec3& y() const { return cameraY; }
		const glm::vec3& z() const { return cameraZ; }
		const glm::vec3& pos() const { return cameraPos; }
	};

	// Projection
	class Projection {
		float f = 0.0f;
		float fov = 0.0f;
		glm::vec2 imgSize = glm::vec2(0);
	public:
		Projection(glm::ivec2 imageSize, float fovDegreesVertical, float focalLength) : imgSize(imageSize), fov(fovDegreesVertical), f(focalLength) {}

		float focalLength() const { return f; }
		float fovDegreesVertical() const { return fov; }
		const glm::vec2& imageSize() const { return imgSize; }
		float aspectRatio() const { return imgSize.x / imgSize.y; }
	};

	Camera(const Frame& frame, const Projection& projection) : frame(frame), projection(projection) {
		float fovRadians = glm::radians(projection.fovDegreesVertical());
		float viewportHeight = 2.f * glm::tan(0.5f * fovRadians) * projection.focalLength();
		float viewportWidth = viewportHeight * projection.aspectRatio();

		glm::vec3 viewportRight = frame.x() * viewportWidth;
		glm::vec3 viewportDown = -frame.y() * viewportHeight;
		glm::vec3 viewportForward = -frame.z() * projection.focalLength();
		pixelDeltaX = viewportRight / projection.imageSize().x;
		pixelDeltaY = viewportDown / projection.imageSize().y;
		glm::vec3 viewportUpperLeft = frame.pos() + viewportForward - 0.5f * (viewportRight + viewportDown);
		pixelUpperLeft = viewportUpperLeft + 0.5f * (pixelDeltaX + pixelDeltaY);
	}

	// Return ray through pixel (x, y) in world coordinates
	Ray getRay(int x, int y, bool jitter = false) const {
		// random offset in [-0.5, -0.5] to [0.5, 0.5]
		glm::vec2 offset = jitter ?
			glm::vec2(random() - 0.5f, random() - 0.5f) :
			glm::vec2(0.f);
		glm::vec3 pixel = pixelUpperLeft + pixelDeltaX * (static_cast<float>(x) + offset.x) + pixelDeltaY * (static_cast<float>(y) + offset.y);
		return Ray(frame.pos(), pixel - frame.pos());
	}
private:
	Frame frame;
	Projection projection;

	// Cached quantities for computing rays. In world coordinates.
	glm::vec3 pixelUpperLeft;
	glm::vec3 pixelDeltaX;
	glm::vec3 pixelDeltaY;
};