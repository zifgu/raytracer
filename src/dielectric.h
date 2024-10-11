#pragma once

#include "material.h"

class Dielectric : public Material {
	// Index of refraction relative to enclosing media - usually vacuum or air, but may be otherwise
	float indexOfRefraction = 1.f;

	// Schlick approximation of Fresnel reflection
	// iorRatio = ior on incident side / ior on transmitted side
	static float fresnelReflectance(float cosTheta, float iorRatio) {
		float r0 = (1 - iorRatio) / (1 + iorRatio);
		r0 = r0 * r0;
		return r0 + (1.f - r0) * glm::pow(1.f - cosTheta, 5);
	}

	// incident, normal assumed to be normalized
	static glm::vec3 refract(glm::vec3 incident, glm::vec3 normal, float iorRatio) {
		float cosTheta = glm::dot(-incident, normal);
		float sinTheta = glm::sqrt(1.f - cosTheta * cosTheta);

		bool totalInternalReflection = iorRatio * sinTheta > 1.f;
		if (totalInternalReflection || random() < fresnelReflectance(cosTheta, iorRatio)) {
			return glm::reflect(incident, normal);
		}

		glm::vec3 perp = iorRatio * (incident + cosTheta * normal);
		glm::vec3 parallel = -glm::sqrt(glm::max(1.f - glm::dot(perp, perp), 0.f)) * normal;
		return perp + parallel;
	}
public:
	Dielectric(float indexOfRefraction) : indexOfRefraction(indexOfRefraction) {}

	bool scatter(const Ray& ray, const Hittable::HitRecord& hit, glm::vec3& attenuation, Ray& scatteredRay) const {
		attenuation = glm::vec3(1.f);
		float relativeIOR = hit.frontFace ? 1.f / indexOfRefraction : indexOfRefraction;
		glm::vec3 refractDirection = refract(glm::normalize(ray.direction()), hit.normal, relativeIOR);
		scatteredRay = Ray(hit.point, refractDirection);
		return true;
	}
};