#pragma once

#include "common.h"
#include "hittable.h"

class Transform : public Hittable {
	glm::vec3 m_translation = glm::vec3(0.f);
	glm::mat3 m_rotation = glm::mat3(1.f);
	glm::vec3 m_scale = glm::vec3(1.f);
	std::shared_ptr<Hittable> m_object = nullptr;

	AABox m_bbox = AABox::empty;

	glm::vec3 transformPoint(const glm::vec3& point) const {
		return m_translation + m_rotation * (m_scale * point);
	}
	glm::vec3 transformDirection(const glm::vec3& dir) const {
		return m_rotation * dir;
	}
	glm::vec3 inverseTransformPoint(const glm::vec3& point) const {
		return (glm::inverse(m_rotation) * (point - m_translation)) / m_scale;
	}
	glm::vec3 inverseTransformDirection(const glm::vec3& dir) const {
		return glm::inverse(m_rotation) * dir;
	}

	void setBBox() {
		m_bbox = AABox::empty;
		AABox objectBox = m_object->boundingBox();
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					float x = i == 0 ? objectBox.x().min() : objectBox.x().max();
					float y = j == 0 ? objectBox.y().min() : objectBox.y().max();
					float z = k == 0 ? objectBox.z().min() : objectBox.z().max();
					glm::vec3 corner = transformPoint(glm::vec3(x, y, z));
					m_bbox.expand(corner);
				}
			}
		}
	}
public:
	Transform(std::shared_ptr<Hittable> obj, glm::vec3 translation = glm::vec3(0.f), glm::mat3 rotation = glm::mat3(1.f), glm::vec3 scale = glm::vec3(1.f)) : m_object(obj), m_translation(translation), m_rotation(rotation), m_scale(scale) {
		setBBox();
	}

	bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const override {
		Ray transformedRay = Ray(inverseTransformPoint(ray.origin()), inverseTransformDirection(ray.direction()));

		if (!m_object->hit(transformedRay, tRange, hit))
			return false;

		hit.point = transformPoint(hit.point);
		hit.normal = transformDirection(hit.normal);

		return true;
	}

	AABox boundingBox() const override {
		return m_bbox;
	}
};