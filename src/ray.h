#pragma once

#include "common.h"

class Ray {
private:
	glm::vec3 m_origin = glm::vec3(0.f);
	glm::vec3 m_direction = glm::vec3(0.f);
public:
	Ray() {}
	Ray(const glm::vec3& origin, const glm::vec3& direction) : m_origin(origin), m_direction(direction) {}
	const glm::vec3& origin() const { return m_origin; }
	const glm::vec3& direction() const { return m_direction; }
	glm::vec3 at(float t) const { return m_origin + t * m_direction; }
};