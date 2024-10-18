#pragma once

#include <vector>
#include <memory>

#include "hittable.h"

class HittableList : public Hittable {
	std::vector<std::shared_ptr<Hittable>> m_objects;
	AABox m_bbox = AABox::empty;
public:
	void clear() {
		m_objects.clear();
		m_bbox = AABox::empty;
	}
	void add(std::shared_ptr<Hittable> object) {
		m_objects.push_back(object);
		m_bbox.expand(object->boundingBox());
	}
	const std::vector<std::shared_ptr<Hittable>>& objects() const { return m_objects; }
	bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const override {
		bool hitAnything = false;
		float closestT = tRange.max();
		HitRecord tempHit;

		for (const std::shared_ptr<Hittable>& object : m_objects) {
			if (object->hit(ray, Interval(tRange.min(), closestT), tempHit)) {
				hitAnything = true;
				closestT = tempHit.t;
				hit = tempHit;
			}
		}

		return hitAnything;
	}

	AABox boundingBox() const override { return m_bbox; }
};