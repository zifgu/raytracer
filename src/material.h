#pragma once

#include "hittable.h"

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& ray, const Hittable::HitRecord& hit, glm::vec3& attenuation, Ray& scatteredRay) const = 0;
};