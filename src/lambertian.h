#pragma once

#include "material.h"
#include "texture.h"

class Lambertian : public Material {
	std::shared_ptr<Texture> m_texture = nullptr;
public:
	Lambertian(glm::vec3 albedo) : m_texture(std::make_shared<SolidColorTexture>(albedo)) {}
	Lambertian(std::shared_ptr<Texture> texture) : m_texture(texture) {}

	bool scatter(const Ray& ray, const Hittable::HitRecord& hit, glm::vec3& attenuation, Ray& scatteredRay) const override {
		glm::vec3 scatterDirection = hit.normal + randomOnSphere();
		scatteredRay = Ray(hit.point, scatterDirection);
		attenuation = m_texture->value(hit.uv, hit.point);
		return true;
	}
};