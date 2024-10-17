#pragma once

#include "hittable.h"

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& ray, const Hittable::HitRecord& hit, glm::vec3& attenuation, Ray& scatteredRay) const {
		return false;
	}
	virtual glm::vec3 emitted(const glm::vec2& uv, const glm::vec3& p) const {
		return glm::vec3(0.f);
	}
};