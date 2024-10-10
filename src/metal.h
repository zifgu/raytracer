#pragma once

#include "material.h"

class Metal : public Material {
	glm::vec3 albedo = glm::vec3(0.f);
	float fuzziness = 0.f;
public:
	Metal(glm::vec3 albedo, float fuzziness) : albedo(albedo), fuzziness(fuzziness) {}

	bool scatter(const Ray& ray, const Hittable::HitRecord& hit, glm::vec3& attenuation, Ray& scatteredRay) const override {
		glm::vec3 scatterDirection = glm::reflect(ray.direction(), hit.normal);
		scatterDirection = glm::normalize(scatterDirection);
		scatterDirection += randomOnSphere() * fuzziness;
		scatteredRay = Ray(hit.point, scatterDirection);
		attenuation = albedo;
		return glm::dot(scatterDirection, hit.normal) > 0.f;
	}
};