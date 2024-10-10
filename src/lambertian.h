#pragma once

#include "material.h"

class Lambertian : public Material {
	glm::vec3 albedo = glm::vec3(0.f);
public:
	Lambertian(glm::vec3 albedo) : albedo(albedo) {}

	bool scatter(const Ray& ray, const Hittable::HitRecord& hit, glm::vec3& attenuation, Ray& scatteredRay) const override {
		glm::vec3 scatterDirection = hit.normal + randomOnSphere();
		scatteredRay = Ray(hit.point, scatterDirection);
		attenuation = albedo;
		return true;
	}
};