#pragma once

#include "common.h"
#include "aabb.h"

class Material;

class Hittable {
public:
	struct HitRecord {
		std::shared_ptr<Material> material;
		glm::vec3 point;
		glm::vec3 normal;	// unit vector
		glm::vec2 uv;
		float t;
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