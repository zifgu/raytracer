#pragma once

#include "common.h"
#include "ray.h"
#include "interval.h"
#include "aabb.h"

class Material;

class Hittable {
public:
	struct HitRecord {
		glm::vec3 point;
		glm::vec3 normal;
		float t;
		std::shared_ptr<Material> material;
		bool frontFace;

		void setFrontFaceAndNormal(const Ray& ray, const glm::vec3& outwardNormal) {
			frontFace = glm::dot(ray.direction(), outwardNormal) < 0.f;
			normal = frontFace ? outwardNormal : -outwardNormal;
		}
	};

	virtual ~Hittable() = default;

	virtual bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const = 0;
	virtual AABox boundingBox() const = 0;
};