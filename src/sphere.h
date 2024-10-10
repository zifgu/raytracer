#pragma once

#include "hittable.h"
#include "material.h"

class Sphere : public Hittable {
	glm::vec3 center = glm::vec3(0.f);
	float radius = 0.f;
	std::shared_ptr<Material> material = nullptr;
public:
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : center(center), radius(radius), material(material){}

	bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const override {
		glm::vec3 oc = center - ray.origin();
		float a = glm::dot(ray.direction(), ray.direction());
		float h = glm::dot(ray.direction(), oc);
		float c = glm::dot(oc, oc) - radius * radius;
		float discriminant = h * h - a * c;
		if (discriminant < 0.f) {
			return false;
		}
		
		float sqrtd = std::sqrt(discriminant);
		float root = (h - sqrtd) / a;
		if (!tRange.surrounds(root)) {
			root = (h + sqrtd) / a;
			if (!tRange.surrounds(root)) {
				return false;
			}
		}

		hit.t = root;
		hit.point = ray.at(hit.t);
		glm::vec3 outwardNormal = (hit.point - center) / radius;
		hit.setFrontFaceAndNormal(ray, outwardNormal);
		hit.material = material;

		return true;
	}
};