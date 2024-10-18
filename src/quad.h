#pragma once

#include "common.h"
#include "hittable.h"
#include "material.h"

class Quad : public Hittable {
	glm::vec3 m_corner = glm::vec3(0.f);
	glm::vec3 m_side1 = glm::vec3(0.f);
	glm::vec3 m_side2 = glm::vec3(0.f);
	std::shared_ptr<Material> m_material = nullptr;

	// Cached quantities for computing intersections.
	glm::vec3 m_planeNormal = glm::vec3(0.f);
	float m_planeD = 0.f;
	glm::vec3 m_planeW = glm::vec3(0.f);

	AABox m_bbox = AABox::empty;

	void setBBox() {
		m_bbox = AABox::empty;
		m_bbox.expand(m_corner);
		m_bbox.expand(m_corner + m_side1);
		m_bbox.expand(m_corner + m_side2);
		m_bbox.expand(m_corner + m_side1 + m_side2);

		// ensure no AABB side is narrower than some delta, in case of axis-aligned quads
		const float delta = 0.0001f;
		for (int comp = 0; comp < 3; ++comp) {
			if (m_bbox.getInterval(comp).size() < delta)
				m_bbox.getInterval(comp).pad(delta);
		}
	}

	void setPlane() {
		glm::vec3 n = glm::cross(m_side1, m_side2);
		m_planeNormal = glm::normalize(n);
		m_planeD = glm::dot(m_planeNormal, m_corner);
		m_planeW = n / glm::dot(n, n);
	}
public:
	Quad(const glm::vec3& q, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> material) : m_corner(q), m_side1(u), m_side2(v), m_material(material) {
		setPlane();
		setBBox();
	}

	const glm::vec3& corner() const { return m_corner; }
	const glm::vec3& side1() const { return m_side1; }
	const glm::vec3& side2() const { return m_side2; }
	const std::shared_ptr<Material> material() const { return m_material; }

	// unit normal of plane containing this quad
	const glm::vec3& planeNormal() const { return m_planeNormal; }
	// value D in the plane equation n_x * x + n_y * y + n_z * z = D
	float planeD() const { return m_planeD; }

	bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const override {
		float denom = glm::dot(m_planeNormal, ray.direction());
		
		const float epsilon = 1e-8f;
		if (glm::abs(denom) < epsilon)
			return false;
		
		float t = (m_planeD - glm::dot(m_planeNormal, ray.origin())) / denom;
		if (!tRange.surrounds(t))
			return false;

		glm::vec3 intersectionPoint = ray.at(t);
		glm::vec3 p = (intersectionPoint - m_corner);
		float u = glm::dot(m_planeW, glm::cross(p, m_side2));
		float v = glm::dot(m_planeW, glm::cross(m_side1, p));

		const Interval unitInterval = Interval(0.f, 1.f);
		if (!unitInterval.contains(u) || !unitInterval.contains(v))
			return false;

		hit.t = t;
		hit.point = ray.at(t);
		hit.setFrontFaceAndNormal(ray, m_planeNormal);
		hit.uv = glm::vec2(u, v);
		hit.material = m_material;

		return true;
	}

	AABox boundingBox() const override {
		return m_bbox;
	}
};