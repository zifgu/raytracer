#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "ray.h"
#include "interval.h"

class Hittable {
public:
	struct HitRecord {
		glm::vec3 point;
		glm::vec3 normal;
		float t;
		bool frontFace;

		void setFrontFaceAndNormal(const Ray& ray, const glm::vec3& outwardNormal) {
			frontFace = glm::dot(ray.direction(), outwardNormal) < 0.f;
			normal = frontFace ? outwardNormal : -outwardNormal;
		}
	};

	virtual ~Hittable() = default;

	virtual bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const = 0;
};