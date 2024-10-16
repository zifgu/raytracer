#pragma once

#include "material.h"

class Metal : public Material {
	glm::vec3 m_albedo = glm::vec3(0.f);
	float m_fuzziness = 0.f;
public:
	Metal(glm::vec3 albedo, float fuzziness) : m_albedo(albedo), m_fuzziness(fuzziness) {}

	bool scatter(const Ray& ray, const Hittable::HitRecord& hit, glm::vec3& attenuation, Ray& scatteredRay) const override {
		glm::vec3 scatterDirection = glm::reflect(ray.direction(), hit.normal);
		scatterDirection = glm::normalize(scatterDirection);
		scatterDirection += randomOnSphere() * m_fuzziness;
		scatteredRay = Ray(hit.point, scatterDirection);
		attenuation = m_albedo;
		return glm::dot(scatterDirection, hit.normal) > 0.f;
	}
};