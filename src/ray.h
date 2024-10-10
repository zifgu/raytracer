#pragma once

#include "common.h"

class Ray {
private:
	glm::vec3 o = glm::vec3(0.f);
	glm::vec3 d = glm::vec3(0.f);
public:
	Ray() {}
	Ray(const glm::vec3& origin, const glm::vec3& direction) : o(origin), d(direction) {}
	const glm::vec3& origin() const { return o; }
	const glm::vec3& direction() const { return d; }
	glm::vec3 at(float t) const { return o + t * d; }
};