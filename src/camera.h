#pragma once

#include "common.h"

class Camera {
public:
	// Camera transformation
	class Frame {
		// Camera position + axes in world coordinates.
		glm::vec3 m_cameraPos;
		glm::vec3 m_cameraX;
		glm::vec3 m_cameraY;
		glm::vec3 m_cameraZ;
	public:
		Frame(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up = glm::vec3(0.f, 1.f, 0.f)) {
			m_cameraPos = lookFrom;
			m_cameraZ = glm::normalize(lookFrom - lookAt);	// camera looks down the negative z axis
			m_cameraX = glm::normalize(glm::cross(up, m_cameraZ));
			m_cameraY = glm::cross(m_cameraZ, m_cameraX);
		}
		const glm::vec3& x() const { return m_cameraX; }
		const glm::vec3& y() const { return m_cameraY; }
		const glm::vec3& z() const { return m_cameraZ; }
		const glm::vec3& pos() const { return m_cameraPos; }
	};

	// Projection
	class Projection {
		float m_focalLength = 0.0f;
		float m_fov = 0.0f;
		glm::vec2 m_imageSize = glm::vec2(0);
	public:
		Projection(glm::ivec2 imageSize, float fovDegreesVertical, float focalLength) : m_imageSize(imageSize), m_fov(fovDegreesVertical), m_focalLength(focalLength) {}

		float focalLength() const { return m_focalLength; }
		float fovDegreesVertical() const { return m_fov; }
		const glm::vec2& imageSize() const { return m_imageSize; }
		float aspectRatio() const { return m_imageSize.x / m_imageSize.y; }
	};

	Camera(const Frame& frame, const Projection& projection) : m_frame(frame), m_projection(projection) {
		float fovRadians = glm::radians(projection.fovDegreesVertical());
		float viewportHeight = 2.f * glm::tan(0.5f * fovRadians) * projection.focalLength();
		float viewportWidth = viewportHeight * projection.aspectRatio();

		glm::vec3 viewportRight = frame.x() * viewportWidth;
		glm::vec3 viewportDown = -frame.y() * viewportHeight;
		glm::vec3 viewportForward = -frame.z() * projection.focalLength();
		m_pixelDeltaX = viewportRight / projection.imageSize().x;
		m_pixelDeltaY = viewportDown / projection.imageSize().y;
		glm::vec3 viewportUpperLeft = frame.pos() + viewportForward - 0.5f * (viewportRight + viewportDown);
		m_pixelUpperLeft = viewportUpperLeft + 0.5f * (m_pixelDeltaX + m_pixelDeltaY);
	}

	// Return ray through pixel (x, y) (from top left) in world coordinates
	// Returned ray direction is exact, not necessarily unit vector
	Ray getRay(int x, int y, bool jitter = false) const {
		// random offset in [-0.5, -0.5] to [0.5, 0.5]
		glm::vec2 offset = jitter ?
			glm::vec2(random() - 0.5f, random() - 0.5f) :
			glm::vec2(0.f);
		glm::vec3 pixel = m_pixelUpperLeft + m_pixelDeltaX * (static_cast<float>(x) + offset.x) + m_pixelDeltaY * (static_cast<float>(y) + offset.y);
		return Ray(m_frame.pos(), pixel - m_frame.pos());
	}
private:
	Frame m_frame;
	Projection m_projection;

	// Cached quantities for computing rays. In world coordinates.
	glm::vec3 m_pixelUpperLeft = glm::vec3(0.f);
	glm::vec3 m_pixelDeltaX = glm::vec3(0.f);
	glm::vec3 m_pixelDeltaY = glm::vec3(0.f);
};