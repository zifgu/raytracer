#pragma once

#include "hittable.h"
#include "material.h"

class Sphere : public Hittable {
	glm::vec3 m_center = glm::vec3(0.f);
	float m_radius = 0.f;
	std::shared_ptr<Material> m_material = nullptr;
public:
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : m_center(center), m_radius(radius), m_material(material){}

	const glm::vec3& center() const { return m_center; }
	const std::shared_ptr<Material> material() const { return m_material; }
	float radius() const { return m_radius; }

	bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const override {
		glm::vec3 oc = m_center - ray.origin();
		float a = glm::dot(ray.direction(), ray.direction());
		float h = glm::dot(ray.direction(), oc);
		float c = glm::dot(oc, oc) - m_radius * m_radius;
		float discriminant = h * h - a * c;
		if (discriminant < 0.f) {
			return false;
		}

		// catch degenerate rays (direction is zero)
		const float epsilon = 1e-8f;
		if (a < epsilon) {
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
		glm::vec3 outwardNormal = (hit.point - m_center) / m_radius;
		hit.setFrontFaceAndNormal(ray, outwardNormal);
		hit.uv = getSphereUV(outwardNormal);
		hit.material = m_material;

		return true;
	}

	AABox boundingBox() const override {
		return AABox(m_center - m_radius, m_center + m_radius);
	}

	// p is a point on the unit sphere centered at 0
	static glm::vec2 getSphereUV(const glm::vec3& p) {
		// uv coordinates should be clockwise around the sphere;
		// also offset by pi to avoid jumping from pi to -pi
		float phi = glm::atan(-p.z, p.x) + pi;

		// uv coordinates should go from bottom of the sphere to top
		float theta = glm::acos(-p.y);

		// scale to [0, 1]
		float u = phi / (2 * pi);
		float v = theta / pi;

		return glm::vec2(u, v);
	}
};